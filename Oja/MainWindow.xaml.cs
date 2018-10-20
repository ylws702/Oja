using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Oja
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string imagePath;
        public MainWindow()
        {
            InitializeComponent();
        }
        private void OpenImageButtun_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog()
            {
                DefaultExt = ".bmp",
                Filter = "Bitmap (*.bmp)|*.bmp"
            };
            if (dialog.ShowDialog() != true)
            {
                return;
            }
            this.imagePath = dialog.FileName;
            try
            {
                BitmapImage bitmap = new BitmapImage(new Uri(this.imagePath));
                this.OriginImage.Source = bitmap;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Exception");
                return;
            }
            this.SaveZipButtun.IsEnabled = true;
        }
        private void SaveZipButtun_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog()
            {
                FileName = "image",
                DefaultExt = ".zg",
                Filter = "Zipped Graphics (*.zg)|*.zg"
            };
            if (dialog.ShowDialog() != true)
            {
                return;
            }
            string zipPath = dialog.FileName;
            Zip net = new Zip(zipPath);
        }

        private void OpenZipButton_Click(object sender, RoutedEventArgs e)
        {
            this.SaveImageButtun.IsEnabled = true;
        }

        private void SaveImageButtun_Click(object sender, RoutedEventArgs e)
        {

        }

    }

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
