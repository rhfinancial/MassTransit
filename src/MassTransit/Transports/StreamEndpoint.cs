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
namespace MassTransit.Transports
{
	using System;
	using System.IO;
	using System.Runtime.Serialization;
	using Context;
	using Internal;
	using log4net;
	using Serialization;

	public class StreamEndpoint :
		AbstractEndpoint
	{
		private static readonly ILog _log = LogManager.GetLogger(typeof (LoopbackEndpoint));
		private bool _disposed;
		private ITransport _errorTransport;
		private ITransport _transport;

		public StreamEndpoint(IEndpointAddress address, IMessageSerializer serializer, ITransport transport, ITransport errorTransport)
			: base(address, serializer)
		{
			_transport = transport;
			_errorTransport = errorTransport;

			SetDisposedMessage();
		}

		public override void Send<T>(T message)
		{
			if (_disposed) throw NewDisposedException();

			Send(message, new PublishContext());
		}

		public override void Send<T>(T message, ISendContext context)
		{
			if (_disposed) throw NewDisposedException();

			_transport.Send(msg =>
				{
					SetOutboundMessageHeaders<T>(context);

					Serializer.Serialize(msg, message, context);

					if (SpecialLoggers.Messages.IsInfoEnabled)
						SpecialLoggers.Messages.InfoFormat("SEND:{0}:{1}", Address, typeof (T).Name);
				}, context);
		}

		public override void Receive(Func<object, Action<object>> receiver, IReceiveContext context)
		{
			if (_disposed) throw NewDisposedException();

			_transport.Receive(ReceiveFromTransport(receiver, context), context);
		}

		public override void Receive(Func<object, Action<object>> receiver, IReceiveContext context, TimeSpan timeout)
		{
			if (_disposed) throw NewDisposedException();

			_transport.Receive(ReceiveFromTransport(receiver, context), context, timeout);
		}

		protected override void Dispose(bool disposing)
		{
			if (_disposed) return;
			if (disposing)
			{
				_transport.Dispose();
				_transport = null;

				_errorTransport.Dispose();
				_errorTransport = null;

				base.Dispose(true);
			}

			_disposed = true;
		}

		private Func<Stream, Action<Stream>> ReceiveFromTransport(Func<object, Action<object>> receiver, IReceiveContext context)
		{
			return message =>
				{
					object messageObj;

					try
					{
						messageObj = Serializer.Deserialize(message, context);
					}
					catch (SerializationException sex)
					{
						if (_log.IsErrorEnabled)
							_log.Error("Unrecognized message " + Address, sex);

						return x => MoveMessageToErrorTransport(x, context);
					}

					if (messageObj == null)
						return null;

					Action<object> receive;
					try
					{
						receive = receiver(messageObj);
						if (receive == null)
						{
							if (_log.IsDebugEnabled)
								_log.DebugFormat("SKIP:{0}:{1}", Address, messageObj.GetType().Name);

							if (SpecialLoggers.Messages.IsInfoEnabled)
								SpecialLoggers.Messages.InfoFormat("SKIP:{0}:{1}", Address, messageObj.GetType().Name);

							return null;
						}
					}
					catch (Exception ex)
					{
						if (_log.IsErrorEnabled)
							_log.Error("An exception was thrown preparing the message consumers", ex);

						MoveMessageToErrorTransport(message, context);
						return null;
					}

					return m =>
						{
							if (_log.IsDebugEnabled)
								_log.DebugFormat("RECV:{0}:{1}", Address, messageObj.GetType().Name);

							if (SpecialLoggers.Messages.IsInfoEnabled)
								SpecialLoggers.Messages.InfoFormat("RECV:{0}:{1}", Address, messageObj.GetType().Name);

							try
							{
								receive(messageObj);
							}
							catch (Exception ex)
							{
								if (_log.IsErrorEnabled)
									_log.Error("An exception was thrown by a message consumer", ex);

								MoveMessageToErrorTransport(m, context);
							}
						};
				};
		}

		private void MoveMessageToErrorTransport(Stream message, IMessageContext context)
		{
			_errorTransport.Send(outbound =>
				{
					var data = new byte[message.Length];
					message.Seek(0, SeekOrigin.Begin);
					message.Read(data, 0, data.Length);

					outbound.Write(data, 0, data.Length);
				}, context);

			if (_log.IsDebugEnabled)
				_log.DebugFormat("MOVE:{0}:{1}", Address, _errorTransport.Address);

			if (SpecialLoggers.Messages.IsInfoEnabled)
				SpecialLoggers.Messages.InfoFormat("MOVE:{0}:{1}", Address, _errorTransport.Address);
		}
	}
}