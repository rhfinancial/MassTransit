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

		/// <summary>
		/// Puts the message back on the queue to be retried later
		/// </summary>
		public void RetryLater()
		{
			if (Message == null)
				throw new InvalidOperationException("RetryLater can only be called when a message is being consumed");

			this.FastInvoke("RetryLater", Message);
		}

		public void Respond<T>(T message) where T : class
		{
			if (ResponseAddress != null)
			{
				GetResponseEndpoint().Send(message, context => context.SetSourceAddress(Bus.Endpoint.Uri));
			}
			else
			{
				Bus.Publish(message);
			}
		}

		/// <summary>
		/// Send a fault to either via publishing or to the Fault Endpoint
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="message">The message to send/publish</param>
		public void GenerateFault<T>(T message)
			where T : class
		{
			if (ResponseAddress != null)
			{
				GetFaultEndpoint().Send(message, context => context.SetSourceAddress(Bus.Endpoint.Uri));
			}
			else
			{
				Bus.Publish(message);
			}
		}

		/// <summary>
		/// Respond to the current inbound message with either a send to the ResponseAddress or a
		/// Publish on the bus that received the message
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="message">The message to send/publish</param>
		/// <param name="contextAction">The action to setup the context on the outbound message</param>
		public void Respond<T>(T message, Action<IPublishContext> contextAction)
			where T : class
		{
			if (ResponseAddress != null)
			{
				GetResponseEndpoint().Send(message, x =>
					{
						x.SetSourceAddress(Bus.Endpoint.Uri);
						contextAction(x as IPublishContext);
					});
			}
			else
			{
				Bus.Publish(message, contextAction);
			}
		}

		private void RetryLater<T>(T message)
			where T : class
		{
			if (Message == null)
				throw new InvalidOperationException("No message is available to retry later");

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

		private IEndpoint GetFaultEndpoint()
		{
			if (FaultAddress == null)
				throw new InvalidOperationException("No fault address was contained in the message");

			return Builder.GetInstance<IEndpointFactory>().GetEndpoint(FaultAddress);
		}

		private IEndpoint GetResponseEndpoint()
		{
			if (ResponseAddress == null)
				throw new InvalidOperationException("No response address was contained in the message");

			return Builder.GetInstance<IEndpointFactory>().GetEndpoint(ResponseAddress);
		}
	}
}