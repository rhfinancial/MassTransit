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
namespace MassTransit.Tests.Serialization
{
	using System;
	using System.Diagnostics;
	using System.IO;
	using System.Text;
	using MassTransit.Internal;
	using MassTransit.Serialization;
	using NUnit.Framework;

	public class SerializationSpecificationBase :
		SerializationTest
	{
		private Uri _sourceUri;
		private Uri _responseUri;
		private Uri _faultUri;
		private Uri _destinationUri;
		private int _retryCount;

		protected void TestSerialization<T>(T message)
		{
			byte[] data;
			var serializer = new XmlMessageSerializer();

			_sourceUri = new Uri("loopback://localhost/source");
			_responseUri = new Uri("loopback://localhost/response");
			_faultUri = new Uri("loopback://localhost/fault");
			_destinationUri = new Uri("loopback://localhost/destination");
			_retryCount = 69;

			_sendContext.SetSourceAddress(_sourceUri);
			_sendContext.SendResponseTo(_responseUri);
			_sendContext.SendFaultTo(_faultUri);
			_sendContext.SetDestinationAddress(_destinationUri);
			_sendContext.SetRetryCount(_retryCount);

			using (MemoryStream output = new MemoryStream())
			{
				serializer.Serialize(output, message, _sendContext);

				data = output.ToArray();
			}

			using (MemoryStream input = new MemoryStream(data))
			{
				object receivedMessage = serializer.Deserialize(input, _receiveContext);

				Assert.AreEqual(message, receivedMessage);
				Assert.AreNotSame(message, receivedMessage);

				Assert.AreEqual(_retryCount, _receiveContext.RetryCount);
				Assert.AreEqual(_sourceUri, _receiveContext.SourceAddress);
				Assert.AreEqual(_responseUri, _receiveContext.ResponseAddress);
				Assert.AreEqual(_faultUri, _receiveContext.FaultAddress);
				Assert.AreEqual(_destinationUri, _receiveContext.DestinationAddress);
				Assert.AreEqual(message.GetType().ToMessageName(), _receiveContext.MessageType);
			}
		}
	}
}