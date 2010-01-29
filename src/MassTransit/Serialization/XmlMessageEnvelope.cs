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
namespace MassTransit.Serialization
{
	using System;
	using System.Xml.Serialization;
	using Context;

	/// <summary>
	/// The envelope that is used to wrap messages serialized using Xml
	/// </summary>
	[XmlRoot(ElementName = "MessageEnvelope")]
	public class XmlMessageEnvelope :
		MessageEnvelope
	{
		protected XmlMessageEnvelope()
		{
		}

		private XmlMessageEnvelope(Type messageType, object message, IMessageContext context)
			: base(context)
		{
			Message = message;

			MessageType = messageType.ToMessageName();
		}

		public object Message { get; private set; }

		public static XmlMessageEnvelope Create<T>(T message, ISendContext context)
		{
			return new XmlMessageEnvelope(typeof (T), message, context);
		}
	}
}