// Copyright 2007-2008 The Apache Software Foundation.
//  
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use 
// this file except in compliance with the License. You may obtain a copy of the 
// License at 
// 
//     http://www.apache.org/licenses/LICENSE-2.0 
// 
// Unless required by applicable law or agreed to in writing, software distributed 
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
// CONDITIONS OF ANY KIND, either express or implied. See the License for the 
// specific language governing permissions and limitations under the License.
namespace MassTransit.Context
{
	using System;
	using System.Collections;
	using System.Web;

	/// <summary>
	/// The default context provider using thread local storage
	/// </summary>
	public class ThreadLocalContextProvider :
		IContextProvider
	{
		[ThreadStatic]
		private static Hashtable _threadStorage;

		private readonly object _hashKey = new object();

		private static bool RunningInWeb
		{
			get { return HttpContext.Current != null; }
		}

		private Hashtable ContextCache
		{
			get
			{
				if (!RunningInWeb)
				{
					return _threadStorage ?? (_threadStorage = CreateContext());
				}

				var hashtable = HttpContext.Current.Items[_hashKey] as Hashtable;
				if (hashtable == null)
				{
					HttpContext.Current.Items[_hashKey] = hashtable = CreateContext();
				}
				return hashtable;
			}
		}

		private static Hashtable CreateContext()
		{
			var hashtable = new Hashtable();

			var publishContext = new PublishContext();

			hashtable[typeof (IPublishContext)] = publishContext;
			hashtable[typeof(ISendContext)] = publishContext;

			var consumeContext = new ConsumeContext();

			hashtable[typeof(IReceiveContext)] = consumeContext;
			hashtable[typeof(IConsumeContext)] = consumeContext;


			return hashtable;
		}

		public void Initialize()
		{
			Retrieve<PublishContext>().Initialize();
		}

		public TResult Context<TContext, TResult>(Func<TContext, TResult> accessor)
		{
			var context = Retrieve<TContext>();

			return accessor(context);
		}

		public void Context<TContext>(Action<TContext> action)
		{
			var context = Retrieve<TContext>();

			action(context);
		}

		private TValue Retrieve<TValue>()
		{
			return (TValue) ContextCache[typeof (TValue)];
		}
	}
}