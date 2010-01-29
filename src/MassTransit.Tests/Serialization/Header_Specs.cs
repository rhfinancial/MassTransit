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
	using System.IO;
	using Context;
	using Magnum.DateTimeExtensions;
	using MassTransit.Serialization;
	using Messages;
	using NUnit.Framework;

	[TestFixture]
	public class Setting_the_message_expiration
		: SerializationSpecificationBase
	{
		private void VerifyMessageHeaderIsPassed(Action<ISendContext> setHeaderAction, Action<IReceiveContext> checkHeaderAction)
		{
			byte[] data;
			var serializer = new XmlMessageSerializer();

			setHeaderAction(_sendContext);

			var message = new PingMessage();

			using (var output = new MemoryStream())
			{
				serializer.Serialize(output, message, _sendContext);

				data = output.ToArray();
			}

			using (var input = new MemoryStream(data))
			{
				serializer.Deserialize(input, _receiveContext);

				checkHeaderAction(_receiveContext);
			}
		}

		[Test]
		public void Should_carry_through_the_message_headers()
		{
			DateTime expiration = 5.Minutes().FromNow();

			VerifyMessageHeaderIsPassed(x => x.ExpiresAt(expiration), x => { Assert.AreEqual(expiration.ToUniversalTime(), x.ExpirationTime); });
		}

		[Test]
		public void Should_not_impact_others_if_not_set()
		{
			VerifyMessageHeaderIsPassed(x => { }, x => { });
		}
	}
}