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
	using Context;

	public static class ExtensionsToMessageEnvelope
	{
		public static void SetReceiveContext(this MessageEnvelopeBase envelope, IReceiveContext context)
		{
			context.Initialize();
			context.SetSourceAddress(envelope.SourceAddress);
			context.SetDestinationAddress(envelope.DestinationAddress);
			context.SetResponseAddress(envelope.ResponseAddress);
			context.SetFaultAddress(envelope.FaultAddress);
			context.SetRetryCount(envelope.RetryCount);
			context.SetMessageType(envelope.MessageType);
			if (envelope.ExpirationTime.HasValue)
				context.SetExpirationTime(envelope.ExpirationTime.Value);
		}

		public static void CopyFrom(this MessageEnvelopeBase envelope, ISendContext context)
		{
			envelope.SourceAddress = context.SourceAddress.ToStringOrNull() ?? envelope.SourceAddress;
			envelope.DestinationAddress = context.DestinationAddress.ToStringOrNull() ?? envelope.DestinationAddress;
			envelope.ResponseAddress = context.ResponseAddress.ToStringOrNull() ?? envelope.ResponseAddress;
			envelope.FaultAddress = context.FaultAddress.ToStringOrNull() ?? envelope.FaultAddress;
			envelope.RetryCount = context.RetryCount;
			envelope.MessageType = context.MessageType ?? envelope.MessageType;
			if (context.ExpirationTime.HasValue)
				envelope.ExpirationTime = context.ExpirationTime.Value;
		}

		public static string ToStringOrNull(this Uri uri)
		{
			return uri == null ? null : uri.ToString();
		}
	}
}