using System;
using System.Net.Sockets;
using System.IO;
using DynaTrace.DynaTraceADK;

namespace DynaTrace.Samples.ServerSide {
	/// <summary>
	/// Our server implementation
	/// </summary>
	class Server {
		public Server() {
		}

		private long multiply(long a, long b) {
			return a * b;
		}

		public void run(int port) {
			TcpListener listen = new TcpListener(port);
			listen.Start();

			Console.WriteLine("server awaiting connections...");

			while (true) {
				TcpClient client = listen.AcceptTcpClient();
                Tagging tagging = MultiAppDotNet.getTagging();
				Console.WriteLine("accepted client");

				BigEndianReaderWriter rw = new BigEndianReaderWriter(client.GetStream());

				// read the dynaTrace tag (if present) and set it
				byte length = rw.ReadByte();
                // if length != 0, the message contains a dynaTrace tag
                if (length != 0) {
                    byte[] tag = rw.ReadBytes(length);
                    tagging.setTag(tag);
                }

                // reading "payload"
				long a = rw.ReadInt64();
				long b = rw.ReadInt64();

                // no dynaTrace tag was sent => we assume that the data itself were used as "custom tag"
                if (length == 0) {
                    byte[] customTag = MultiAppDotNet.getCustomTagFromData(a, b);
                    tagging.setCustomTag(customTag);
                }

				// start server-side PurePath
				tagging.startServerPurePath();

				long r = multiply(a, b);
				Console.WriteLine(a + "*" + b + "=" + r);
				rw.WriteInt64(r);

				// end server-side PurePath
				MultiAppDotNet.getTagging().endServerPurePath();

				client.Close();
			}
		}
	}
}

