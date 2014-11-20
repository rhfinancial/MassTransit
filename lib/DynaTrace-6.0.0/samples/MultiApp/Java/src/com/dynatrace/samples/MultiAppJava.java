package com.dynatrace.samples;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import com.dynatrace.adk.DynaTraceADKFactory;
import com.dynatrace.adk.Tagging;

/**
 * Our sample application's main class
 */
public class MultiAppJava {
	private static final String HOST = "localhost";
	private static final int PORT = 1234;
	
	private static Tagging tagging = null;
	public static Tagging getTagging() {
		return tagging;
	}
	
	public static void main(String[] args) throws IOException {
		// initialize the dynaTrace ADK
		DynaTraceADKFactory.initialize();
		
		// get an instance of the Tagging ADK
		tagging = DynaTraceADKFactory.createTagging();
		
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		char c = '0';
		while (c != 'q') {
			System.out.println("Available Options:");
			System.out.println("  1 ... start server");
			System.out.println("  2 ... do remote call (by dynaTrace tags)");
			System.out.println("  3 ... do remote call (by custom tagging)");
			System.out.println("  q ... quit");
			
			String s = reader.readLine();
			if (s.length() < 1) continue;
			c = s.charAt(0);

			try {
				switch (s.charAt(0)) {
					case '1': new Server().run(PORT); break;
					case '2': new Client().run(HOST, PORT, true); break;
					case '3': new Client().run(HOST, PORT, false); break;
					case 'q': break;
				}
			} catch (Exception e) {
				System.out.println("exception: " + e.getMessage());
			}
		}
		
		// uninitialize the dynaTrace ADK
		DynaTraceADKFactory.uninitialize();
	}
	
	
	/**
	 * 	Helper method - using parts of the data as "custom tags"
	 *	@param a
	 *	@param b
	 *	@return	custom tag, calculated from the input parameters 
	 */
    public static byte[] getCustomTagFromData(long a, long b) {
        byte[] customTag = new byte[8];

        customTag[0] = (byte)((a >> 24) & 0xFF);
        customTag[1] = (byte)((a >> 16) & 0xFF);
        customTag[2] = (byte)((a >>  8) & 0xFF);
        customTag[3] = (byte)((a >>  0) & 0xFF);

        customTag[4] = (byte)((b >> 24) & 0xFF);
        customTag[5] = (byte)((b >> 16) & 0xFF);
        customTag[6] = (byte)((b >>  8) & 0xFF);
        customTag[7] = (byte)((b >>  0) & 0xFF);
        return customTag;
    }
}
