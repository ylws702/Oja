using System;
using System.Runtime.InteropServices;

namespace Oja
{
    /// <summary>
    /// 压缩类
    /// </summary>
    class Zip
    {
        /// <summary>
        /// 输入大小
        /// </summary>
        private const int InputSize = 64;
        /// <summary>
        /// 输出大小
        /// </summary>
        private const int OutputSize = 4;
        /// <summary>
        /// 图像宽度
        /// </summary>
        private const int width = 512;
        /// <summary>
        /// 图像高度
        /// </summary>
        private const int height = 512;

        /// <summary>
        /// 初始化压缩
        /// </summary>
        /// <param name="path">要保存的压缩文件路径</param>
        [DllImport("Oja_x86.dll", EntryPoint = "init", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllInit(
            [MarshalAs(UnmanagedType.AnsiBStr)]
            string path);

        /// <summary>
        /// 压缩完成后清理内存
        /// </summary>
        [DllImport("Oja_x86.dll", EntryPoint = "dispose", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllDispose();

        /// <summary>
        /// 训练
        /// </summary>
        /// <param name="input">训练输入数据</param>
        [DllImport("Oja_x86.dll", EntryPoint = "train", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllTrain(
            [MarshalAs(UnmanagedType.LPArray,SizeConst = InputSize)]
            float[] input);

        /// <summary>
        /// 设置偏移量(用于零均值化)
        /// </summary>
        /// <param name="offset">偏移量</param>
        [DllImport("Oja_x86.dll", EntryPoint = "setOffset", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllSetOffset(
            [MarshalAs(UnmanagedType.LPArray,SizeConst = InputSize)]
            float[] offset);

        /// <summary>
        /// 写入文件头和偏移量
        /// </summary>
        [DllImport("Oja_x86.dll", EntryPoint = "writeHeaderAndOffset", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllWriteHeaderAndOffset();

        /// <summary>
        /// 写入权值
        /// </summary>
        [DllImport("Oja_x86.dll", EntryPoint = "writeWeights", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllWriteWeights();

        /// <summary>
        /// 写入数据
        /// </summary>
        /// <param name="input">输入数据,大小为OutputSize</param>
        [DllImport("Oja_x86.dll", EntryPoint = "write", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllWrite(
            [MarshalAs(UnmanagedType.LPArray,SizeConst = InputSize)]
            float[] input);

        /// <summary>
        /// 初始化压缩文件,路径为<paramref name="path"/>
        /// </summary>
        /// <param name="path">压缩文件路径</param>
        public Zip(string path)
        {
            DllInit(path);
        }

        ~Zip()
        {
            DllDispose();
        }

        /// <summary>
        /// 生成压缩数据并保存
        /// </summary>
        /// <param name="data">图像数据</param>
        /// <exception cref="ArgumentOutOfRangeException">图像数据大小不正确,抛出异常</exception>
        public void Generate(float[,] data)
        {
            //图像数据大小不正确,抛出异常
            if (data.Length != width * height)
            {
                throw new ArgumentOutOfRangeException();
            }
            //数据集各维度期望(偏移量)
            float[] offset = new float[InputSize];
            for (int i = 0; i < 64; i++)
            {
                for (int j = 0; j < 64; j++)
                {
                    int xStart = 8 * i;
                    int yStart = 8 * j;
                    for (int x = 0; x < 8; x++)
                    {
                        for (int y = 0; y < 8; y++)
                        {
                            offset[8 * x + y] += data[xStart + x, yStart + y]; ;
                        }
                    }
                }
            }
            for (int i = 0; i < 64; i++)
            {
                offset[i] /= 4096;
            }
            //训练网络
            float[] block = new float[InputSize];
            for (int i = 0; i < 64; i++)
            {
                for (int j = 0; j < 64; j++)
                {
                    int xStart = 8 * i;
                    int yStart = 8 * j;
                    for (int x = 0; x < 8; x++)
                    {
                        for (int y = 0; y < 8; y++)
                        {
                            block[8 * x + y] = data[xStart + x, yStart + y] - offset[8 * x + y];
                        }
                    }
                    DllTrain(block);
                }
            }
            //设置偏移量
            DllSetOffset(offset);
            //写入头文件和偏移量
            DllWriteHeaderAndOffset();
            //写入权重
            DllWriteWeights();
            //写入压缩数据
            for (int i = 0; i < 64; i++)
            {
                for (int j = 0; j < 64; j++)
                {
                    int xStart = 8 * i;
                    int yStart = 8 * j;
                    for (int x = 0; x < 8; x++)
                    {
                        for (int y = 0; y < 8; y++)
                        {
                            block[8 * x + y] = data[xStart + x, yStart + y] - offset[8 * x + y];
                        }
                    }
                    DllWrite(block);
                }
            }
        }
    }
}
