using System;
using System.IO;

namespace DynaTrace.Samples {
	/// <summary>
	/// Helper class for reading and writing to a Stream in network byte order
	/// </summary>
	public class BigEndianReaderWriter {
		private BinaryReader reader;
		private BinaryWriter writer;

		public BigEndianReaderWriter(Stream stream) {
			reader = new BinaryReader(stream);
			writer = new BinaryWriter(stream);
		}

		private static ushort swap16(ushort value) {
			return (ushort)((value >> 8) | (value << 8));
		}

		private static uint swap32(uint value) {
			return (uint)swap16((ushort)(value & 0xFFFF)) << 16 | (uint)swap16((ushort)(value >> 16));
		}

		private static ulong swap64(ulong value) {
			return (ulong)swap32((uint)(value & 0xFFFFFFFF)) << 32 | (ulong)swap32((uint)(value >> 32));
		}

		public byte ReadByte() { return reader.ReadByte(); }
		public byte[] ReadBytes(int count) { return reader.ReadBytes(count); }
		public long ReadInt64() { return (long)swap64((ulong)reader.ReadInt64()); }

		public void WriteByte(byte value) { writer.Write(value); }
		public void WriteBytes(byte[] value) { writer.Write(value); }
		public void WriteInt64(long value) { writer.Write((long)swap64((ulong)value)); }
	}
}
