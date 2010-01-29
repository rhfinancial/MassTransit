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
	using System.Collections.Generic;

	public class PublishContext :
		AbstractMessageContext,
		IPublishContext
	{
		private readonly HashSet<Uri> _endpoints = new HashSet<Uri>();
		private Action<object, IEndpoint> _eachSubscriberAction = Ignore;
		private Action<object> _noSubscribersAction = Ignore;

		public void SendResponseTo(IServiceBus bus)
		{
			SetResponseAddress(bus.Endpoint.Uri);
		}

		public void SendResponseTo(IEndpoint endpoint)
		{
			SetResponseAddress(endpoint.Uri);
		}

		public void SendResponseTo(Uri uri)
		{
			SetResponseAddress(uri);
		}

		public void SendFaultTo(IServiceBus bus)
		{
			SetFaultAddress(bus.Endpoint.Uri);
		}

		public void SendFaultTo(IEndpoint endpoint)
		{
			SetFaultAddress(endpoint.Uri);
		}

		public void SendFaultTo(Uri uri)
		{
			SetFaultAddress(uri);
		}

		public void ExpiresAt(DateTime value)
		{
			SetExpirationTime(value);
		}

		public void NotifyForMessageConsumer<T>(T message, IEndpoint endpoint)
		{
			_endpoints.Add(endpoint.Uri);

			_eachSubscriberAction(message, endpoint);
		}

		public bool WasEndpointAlreadySent(Uri endpointUri)
		{
			return _endpoints.Contains(endpointUri);
		}

		public void NotifyNoSubscribers<T>(T message)
		{
			_noSubscribersAction(message);
		}

		public override void Initialize()
		{
			_noSubscribersAction = Ignore;
			_eachSubscriberAction = Ignore;
			_endpoints.Clear();

			base.Initialize();
		}

		public void IfNoSubscribers<T>(Action<T> action)
		{
			_noSubscribersAction = x => action((T) x);
		}

		public void ForEachSubscriber<T>(Action<T, IEndpoint> action)
		{
			_eachSubscriberAction = (message, endpoint) => action((T) message, endpoint);
		}

		private static void Ignore<T>(T message)
		{
		}

		private static void Ignore<T>(T message, IEndpoint endpoint)
		{
		}
	}
}