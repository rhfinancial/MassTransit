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
	using Magnum.Reflection;

	public class ConsumeContext :
		AbstractMessageContext,
		IReceiveContext
	{
		/// <summary>
		/// The bus on which the message was received
		/// </summary>
		public IServiceBus Bus { get; private set; }

		/// <summary>
		/// The message that was received
		/// </summary>
		public object Message { get; private set; }

		/// <summary>
		/// The object builder (container)
		/// </summary>
		public IObjectBuilder Builder { get; private set; }

		/// <summary>
		/// Puts the message back on the queue to be retried later
		/// </summary>
		public void RetryLater()
		{
			if (Message == null)
				throw new InvalidOperationException("RetryLater can only be called when a message is being consumed");

			this.FastInvoke("RetryLater", Message);
		}

		private void RetryLater<T>(T message)
			where T : class
		{
			Bus.Endpoint.Send(message, x =>
				{
					x.SetSourceAddress(SourceAddress);
					x.SetDestinationAddress(DestinationAddress);
					x.SendResponseTo(ResponseAddress);
					x.SendFaultTo(FaultAddress);
					x.SetRetryCount(RetryCount + 1);

					if (ExpirationTime.HasValue)
						x.SetExpirationTime(ExpirationTime.Value);
				});
		}

		public void SetBus(IServiceBus bus)
		{
			Bus = bus;
		}

		public void SetMessage(object message)
		{
			Message = message;
		}

		public void SetObjectBuilder(IObjectBuilder objectBuilder)
		{
			Builder = objectBuilder;
		}
	}
}