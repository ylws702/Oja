using System;
using System.Runtime.InteropServices;

namespace Oja
{
    class Zip
    {
        private const int InputSize = 64;
        private const int OutputSize = 4;
        private const int width = 512;
        private const int height = 512;

        [DllImport("Oja_x86.dll", EntryPoint = "init", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllInit(string path);

        [DllImport("Oja_x86.dll", EntryPoint = "dispose", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllDispose();
        
        [DllImport("Oja_x86.dll", EntryPoint = "train", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllTrain(
            [MarshalAs(UnmanagedType.LPArray,SizeConst = InputSize)]
            float[] input);

        [DllImport("Oja_x86.dll", EntryPoint = "setOffset", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllSetOffset(
            [MarshalAs(UnmanagedType.LPArray,SizeConst = InputSize)]
            float[] offset);

        [DllImport("Oja_x86.dll", EntryPoint = "writeHeaderAndOffset", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllWriteHeaderAndOffset();

        [DllImport("Oja_x86.dll", EntryPoint = "writeWeights", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllWriteWeights();

        [DllImport("Oja_x86.dll", EntryPoint = "write", CallingConvention = CallingConvention.Cdecl)]
        private static extern void DllWrite(
            [MarshalAs(UnmanagedType.LPArray,SizeConst = InputSize)]
            float[] input);

        public Zip(string path)
        {
            DllInit(path);
        }

        ~Zip()
        {
            DllDispose();
        }

        public void Generate(float[,] data)
        {
            if (data.Length != width * height)
            {
                throw new ArgumentOutOfRangeException();
            }
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
            DllSetOffset(offset);
            DllWriteHeaderAndOffset();
            DllWriteWeights();
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
