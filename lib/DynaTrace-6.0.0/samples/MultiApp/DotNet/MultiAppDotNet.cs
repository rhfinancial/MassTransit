using System;
using DynaTrace.DynaTraceADK;
using DynaTrace.Samples.ClientSide;
using DynaTrace.Samples.ServerSide;

namespace DynaTrace.Samples {
	/// <summary>
	/// Our sample application's main class
	/// </summary>
	class MultiAppDotNet {
		private const string HOST = "localhost";
		private const int PORT = 1234;

		private static Tagging tagging = null;
		public static Tagging getTagging() {
			return tagging;
		}
		
		public static void Main(string[] args) {
			// initialize the dynaTrace ADK
			DynaTraceADKFactory.initialize();

			// get an instance of the Tagging ADK
			tagging = DynaTraceADKFactory.createTagging();

			Char c = '0';
			while (c != 'q') {
                Console.WriteLine("Available Options:");
                Console.WriteLine("  1 ... start server");
				Console.WriteLine("  2 ... do remote call (by dynaTrace tags)");
                Console.WriteLine("  3 ... do remote call (by custom tagging)");
                Console.WriteLine("  q ... quit");

				String s = Console.ReadLine();
				if (s.Length < 1) continue;
				c = s[0];

				try {
					switch (c) {
						case '1': new Server().run(PORT); break;
						case '2': new Client().run(HOST, PORT, true); break;
                        case '3': new Client().run(HOST, PORT, false); break;
                        case 'q': break;
					}
				} catch (Exception e) {
					Console.WriteLine("exception: " + e.Message);
				}
			}

			// uninitialize the dynaTrace ADK
			DynaTraceADKFactory.uninitialize();
		}

        /// <summary>
        /// Helper method - using parts of the data as "custom tags"
        /// </summary>
        /// 
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
}
