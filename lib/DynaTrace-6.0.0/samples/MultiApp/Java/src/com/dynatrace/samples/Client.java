package com.dynatrace.samples;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Random;

import com.dynatrace.adk.Tagging;
import com.dynatrace.adk.Tagging.CustomTag;

/**
 * Our client implementation
 */
public class Client {
	private static final Random random = new Random();
	
	public void run(String host, int port, boolean use_dt_tag) throws IOException {
		Socket client = new Socket(host, port);
		Tagging tagging = MultiAppJava.getTagging();
		
		long a = random.nextInt();
		long b = random.nextInt();
		
		DataInputStream dis = new DataInputStream(client.getInputStream());
		DataOutputStream dos = new DataOutputStream(client.getOutputStream());
		
	    // Case A: Use dynaTrace tagging for linking remote calls:
		if (use_dt_tag) {
			// get the current dynaTrace tag and transmit it to the server
			byte[] tag = tagging.getTag();
			
			// insert a synchronous link node
			tagging.linkClientPurePath(false, tag);
			
			dos.writeByte(tag.length);
			dos.write(tag);
		}
		
	    // Case B: Use "custom tagging" for linking remote calls:
		else {
			// Create a "custom tag" - this is unique to this demo app. Normally the custom tag
 			// is a unique identifier that already exists in the monitored applications architecture.
			// Here we're using the data itself as "custom tag" (getCustomTagFromData here is just some kind of type cast)
            CustomTag customTag = tagging.createCustomTag(MultiAppJava.getCustomTagFromData(a, b));

            // insert a synchronous link node using our specific customTag
            tagging.linkClientPurePath(false, customTag);
            
            // write a 0 byte, (no need to send extra data, since the customTag is part of the paylod yet):
            dos.writeByte((byte)0);
		}

		// This is the application logic sending the application payload to the remote process.
		dos.writeLong(a);
		dos.writeLong(b);
		
		// read response from server
		long r = dis.readLong();
		
		System.out.println(a + " * " + b + " = " + r);
				
		client.close();
	}
}
