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
        private const int InputSize = 64;
        private const int OutputSize = 4;
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
        [DllImport("Oja_x86.dll", EntryPoint = "zip", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Zip(
            [MarshalAs(UnmanagedType.LPStr)]
            string savePath,
            [MarshalAs(UnmanagedType.LPArray,SizeConst = InputSize)]
            int[] input,
            [MarshalAs(UnmanagedType.LPArray,SizeConst = OutputSize)]
            ref int[] output);
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
            string zpPath = dialog.FileName;
            int[] input = new int[InputSize];
            int[] output = new int[OutputSize];
            Zip("131464", input, ref output);
        }

        private void OpenZipButton_Click(object sender, RoutedEventArgs e)
        {
            this.SaveImageButtun.IsEnabled = true;
        }

        private void SaveImageButtun_Click(object sender, RoutedEventArgs e)
        {

        }

    }
}
