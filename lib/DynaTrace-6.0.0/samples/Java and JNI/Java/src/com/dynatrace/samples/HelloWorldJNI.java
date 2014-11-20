package com.dynatrace.samples;

public class HelloWorldJNI {
	public static void main(String [] args) {
		NativeCalls.pre();
		NativeCalls.say("Hello World!");
		NativeCalls.post();
	}
}
