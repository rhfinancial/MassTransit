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
namespace MassTransit.Tests
{
	using System;
	using Context;
	using NUnit.Framework;
	using TestFramework;

	[TestFixture]
	public class Using_a_default_context_provider :
		Given_a_standalone_service_bus
	{
		private Uri _sourceAddress;
		private Uri _destinationAddress;
		private Uri _faultAddress;
		private Uri _responseAddress;

		[TestFixtureSetUp]
		public void Setup()
		{
			_sourceAddress = new Uri("msmq://localhost/mt_client");
			_destinationAddress = new Uri("msmq://localhost/mt_server");
			_faultAddress = new Uri("msmq://localhost/mt_fault");
			_responseAddress = new Uri("msmq://localhost/mt_response");

			LocalBus.SendContext(x =>
			{
				x.SetSourceAddress(_sourceAddress);
				x.SetDestinationAddress(_destinationAddress);
				x.SetFaultAddress(_faultAddress);
				x.SetResponseAddress(_responseAddress);
			});
		}

		[Test]
		public void Should_allow_access_to_the_source_address()
		{
			LocalBus.PublishContext(x => x.SourceAddress).ShouldEqual(_sourceAddress);
		}

		[Test]
		public void Should_allow_access_to_the_destination_address()
		{
			LocalBus.PublishContext(x => x.DestinationAddress).ShouldEqual(_destinationAddress);
		}
		[Test]
		public void Should_allow_access_to_the_fault_address()
		{
			LocalBus.PublishContext(x => x.FaultAddress).ShouldEqual(_faultAddress);
		}

		[Test]
		public void Should_allow_access_to_the_response_address()
		{
			LocalBus.PublishContext(x => x.ResponseAddress).ShouldEqual(_responseAddress);
		}
	}
}