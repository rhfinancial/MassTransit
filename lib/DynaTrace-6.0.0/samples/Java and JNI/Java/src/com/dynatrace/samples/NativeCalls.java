package com.dynatrace.samples;

import com.dynatrace.adk.DynaTraceADKFactory;
import com.dynatrace.adk.Tagging;

public class NativeCalls {
	public static native void say(String what);

	static {
		try {
			System.loadLibrary("helloworld");
			initialize();
			Runtime.getRuntime().addShutdownHook(new Thread() {
				// Not Java 1.4 compatible: @Override
				public void run() {
					uninitialize();
				}
			});
		} catch (LinkageError e) {
			e.printStackTrace();
		}
	}
	
	private static Tagging tagging; 
	static {
		DynaTraceADKFactory.initialize();
		tagging = DynaTraceADKFactory.createTagging();
	}
	
	private static native void initialize();
	private static native void uninitialize();
	private static native void setTag(byte [] tag);
	private static native void startServerPurePath();
	private static native void endServerPurePath();

	public static void pre() {
		NativeCalls.setTag(tagging.getTag());
		tagging.linkClientPurePath(false);
		NativeCalls.startServerPurePath();
	}
	
	public static void post() {
		NativeCalls.endServerPurePath();
	}
}
