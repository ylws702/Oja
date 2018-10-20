using System.Runtime.InteropServices;

namespace Oja
{
    static class Unzip
    {
        [DllImport("Oja_x86.dll", EntryPoint = "unzip", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllUnzip(
            string path,
            [MarshalAs(UnmanagedType.LPArray,SizeConst = 512*512)]
            ref float[] output);

        public static float[] GetData(string zipPath)
        {
            float[] data = new float[512*512];
            DllUnzip(zipPath, ref data);
            return data;
        }
    }
}
