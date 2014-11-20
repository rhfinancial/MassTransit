using System;
using System.Net.Sockets;
using System.IO;
using DynaTrace.DynaTraceADK;

#pragma warning disable 0162

namespace DynaTrace.Samples.ClientSide {

	/// <summary>
	/// Our client implementation
	/// </summary>
	class Client {
        private static Random random = new Random();

		public void run(string host, int port, bool use_dt_tag) {
			TcpClient client = new TcpClient(host, port);
            Tagging tagging = MultiAppDotNet.getTagging();

			long a = random.Next();
			long b = random.Next();

			BigEndianReaderWriter rw = new BigEndianReaderWriter(client.GetStream());

            // Either use dynaTrace tags or "custom tags" (passive tagging) for linking remote calls
            if (use_dt_tag) {
                // get the current dynaTrace tag and transmit it to the server
                byte[] tag = tagging.getTag();
                rw.WriteByte((byte)tag.Length);
                rw.WriteBytes(tag);
                // insert a synchronous link node
                tagging.linkClientPurePath(false);
            }
            else {
                // when using "custom tagging" we're using the data itself as "custom tag"
                CustomTag customTag = tagging.createCustomTag(MultiAppDotNet.getCustomTagFromData(a, b));
                // write a 0 byte, indicating that *no* tag is being sent
                rw.WriteByte((byte)0);
                // insert a synchronous link node using our specific customTag
                tagging.linkClientPurePath(false, customTag);
            }

            // send "payload"
			rw.WriteInt64(a);
			rw.WriteInt64(b);

            // read response from server
			long r = rw.ReadInt64();

			Console.WriteLine(a + "*" + b + "=" + r);

			client.Close();
		}
	}
}
