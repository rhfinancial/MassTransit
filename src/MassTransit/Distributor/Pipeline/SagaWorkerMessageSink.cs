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
namespace MassTransit.Distributor.Pipeline
{
	using System;
	using System.Collections.Generic;
	using Context;
	using Internal;
	using MassTransit.Pipeline;
	using Messages;
	using Saga;

	public class SagaWorkerMessageSink<TSaga, TMessage> :
		IPipelineSink<Distributed<TMessage>>
		where TSaga : SagaStateMachine<TSaga>, ISaga
		where TMessage : class
	{
		private readonly ISagaWorker<TSaga> _worker;
		private IPipelineSink<TMessage> _sink;
		private readonly IServiceBus _bus;

		public SagaWorkerMessageSink(ISagaWorker<TSaga> worker, IPipelineSink<TMessage> sink, IServiceBus bus)
		{
			_worker = worker;
			_sink = sink;
			_bus = bus;
		}

		public void Dispose()
		{
			_sink.Dispose();
			_sink = null;
		}

		public IEnumerable<Action<Distributed<TMessage>>> Enumerate(Distributed<TMessage> item)
		{
			if (!_worker.CanAcceptMessage(item))
				yield break;

			_bus.ReceiveContext(x => x.SetResponseAddress(item.ResponseAddress));
		
			foreach (var sinkAction in _sink.Enumerate(item.Payload))
			{
				Action<TMessage> action = sinkAction;

				yield return message =>
					{
						_worker.IncrementInProgress();
						try
						{
							_worker.ConsumingMessage(message);

							action(message.Payload);
						}
						finally
						{
							_worker.DecrementInProgress();
						}
					};
			}
		}

		public bool Inspect(IPipelineInspector inspector)
		{
			return inspector.Inspect(this, () => _sink.Inspect(inspector));
		}
	}
}