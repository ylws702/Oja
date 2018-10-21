using System.Runtime.InteropServices;

namespace Oja
{
    /// <summary>
    /// 解压类
    /// </summary>
    static class Unzip
    {
        /// <summary>
        /// 解压<paramref name="path"/>文件为float数组至<paramref name="output"/>,每64元素为一个块的数据
        /// </summary>
        /// <param name="path">压缩文件路径</param>
        /// <param name="output">解压出的数据</param>
        [DllImport("Oja_x86.dll", EntryPoint = "unzip", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllUnzip(
            [MarshalAs(UnmanagedType.AnsiBStr)]
            string path,
            [MarshalAs(UnmanagedType.LPArray,SizeConst = 512*512)]
            ref float[] output);

        /// <summary>
        /// 解压<paramref name="zipPath"/>文件为float数组,每64元素为一个块的数据
        /// </summary>
        /// <param name="zipPath">压缩文件路径</param>
        /// <returns>解压出的数据</returns>
        public static float[] GetData(string zipPath)
        {
            float[] data = new float[512*512];
            DllUnzip(zipPath, ref data);
            return data;
        }
    }
}
