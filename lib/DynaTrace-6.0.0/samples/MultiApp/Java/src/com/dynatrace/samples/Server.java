package com.dynatrace.samples;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import com.dynatrace.adk.Tagging;

/**
 * Our server implementation
 */
public class Server {
	private long multiply(long a, long b) {
		return a * b;
	}

	public void run(int port) throws IOException {
		ServerSocket listen = new ServerSocket(port);
		
		System.out.println("server awaiting connections...");
		
		while (true) {
			Socket client = listen.accept();
			Tagging tagging = MultiAppJava.getTagging();
			System.out.println("accepted client");
			
			DataInputStream  dis = new DataInputStream(client.getInputStream());
			DataOutputStream dos = new DataOutputStream(client.getOutputStream());

			// read the dynaTrace tag and set it
			byte length = dis.readByte();
			boolean customTagging = length == 0;
			// if length != 0, the message contains a dynaTrace tag
			if (length != 0) {
				byte[] tag = new byte[length];
				do {
					length -= dis.read(tag, tag.length-length, length);
				} while (length > 0);
				// tell dynaTrace the tag value
				tagging.setTag(tag);
			}

			// read "payload"
			long a = dis.readLong();
			long b = dis.readLong();

            // no dynaTrace tag was sent => we assume that the data itself were used as "custom tag"
            // (i.e. "custom tag" is part of the payload)
            if (customTagging) {
                byte[] customTag = MultiAppJava.getCustomTagFromData(a, b);
                tagging.setCustomTag(customTag);
            }

			// start server-side PurePath
			tagging.startServerPurePath();
			
			long r = multiply(a, b);
			System.out.println(a + "*" + b + "=" + r);			
			dos.writeLong(r);

			// end server-side PurePath
			tagging.endServerPurePath();
			
			client.close();
		}
	}
}
