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

	public static class ContextExtensions
	{
		public static TResult ReceiveContext<TResult>(this IServiceBus bus, Func<IReceiveContext, TResult> accessor)
		{
			if (bus == null)
				throw new ArgumentNullException("bus");

			return bus.Context(accessor);
		}

		public static void ReceiveContext(this IServiceBus bus, Action<IReceiveContext> accessor)
		{
			if (bus == null)
				throw new ArgumentNullException("bus");

			bus.Context(accessor);
		}

		public static TResult ConsumeContext<TResult>(this IServiceBus bus, Func<IConsumeContext, TResult> accessor)
		{
			if (bus == null)
				throw new ArgumentNullException("bus");

			return bus.Context(accessor);
		}

		public static void ConsumeContext(this IServiceBus bus, Action<IConsumeContext> accessor)
		{
			if (bus == null)
				throw new ArgumentNullException("bus");

			bus.Context(accessor);
		}

		public static TResult PublishContext<TResult>(this IServiceBus bus, Func<IPublishContext, TResult> accessor)
		{
			if (bus == null)
				throw new ArgumentNullException("bus");

			return bus.Context(accessor);
		}

		public static void PublishContext(this IServiceBus bus, Action<IPublishContext> accessor)
		{
			if (bus == null)
				throw new ArgumentNullException("bus");

			bus.Context(accessor);
		}

		public static void SendContext(this IServiceBus bus, Action<ISendContext> accessor)
		{
			if (bus == null)
				throw new ArgumentNullException("bus");

			bus.Context(accessor);
		}

		public static TResult MessageContext<TResult>(this IServiceBus bus, Func<IMessageContext, TResult> accessor)
		{
			if (bus == null)
				throw new ArgumentNullException("bus");

			return bus.Context(accessor);
		}
	}
}