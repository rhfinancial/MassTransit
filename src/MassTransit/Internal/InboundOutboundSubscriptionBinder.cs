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
namespace MassTransit.Internal
{
	using Pipeline;

	public class InboundOutboundSubscriptionBinder :
		ISubscriptionEvent
	{
		private readonly IEndpoint _localEndpoint;
		private readonly IMessagePipeline _outbound;
		private readonly IServiceBus _bus;

		public InboundOutboundSubscriptionBinder(IMessagePipeline outbound, IServiceBus bus, IEndpoint localEndpoint)
		{
			_outbound = outbound;
			_bus = bus;
			_localEndpoint = localEndpoint;
		}

		public UnsubscribeAction SubscribedTo<T>()
			where T : class
		{
			return _outbound.Subscribe<T>(_bus, _localEndpoint);
		}

		public UnsubscribeAction SubscribedTo<T, K>(K correlationId)
			where T : class, CorrelatedBy<K>
		{
			return _outbound.Subscribe<T, K>(correlationId, _bus, _localEndpoint);
		}
	}
}