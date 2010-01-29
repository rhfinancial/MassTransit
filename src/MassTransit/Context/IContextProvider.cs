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

	/// <summary>
	/// A context provider is used to obtain context information about the message
	/// </summary>
	public interface IContextProvider
	{
		/// <summary>
		/// Initialize the context for a new operation. This should reset any per-thread context
		/// objects to a newly initialized state.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Returns a value from the specified context, using the current thread context as the key index
		/// </summary>
		/// <typeparam name="TContext">The type of context being requested</typeparam>
		/// <typeparam name="TResult">The type of the property being returned</typeparam>
		/// <param name="accessor">The accessor method to return the value</param>
		/// <returns>The value returned by the accessor function</returns>
		TResult Context<TContext, TResult>(Func<TContext, TResult> accessor);

		/// <summary>
		/// Calls the action with the requested context interface
		/// </summary>
		/// <typeparam name="TContext"></typeparam>
		/// <param name="action"></param>
		void Context<TContext>(Action<TContext> action);
	}
}