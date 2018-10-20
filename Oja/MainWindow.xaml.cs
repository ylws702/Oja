using Microsoft.Win32;
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;

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
            this.Cursor = Cursors.Wait;
            string zipPath = dialog.FileName;
            Zip zip = new Zip(zipPath);
            Bitmap bitmap = ConvertToBitmap(this.OriginImage.Source as BitmapSource);
            Bitmap ConvertToBitmap(BitmapSource bitmapSource)
            {
                var width = bitmapSource.PixelWidth;
                var height = bitmapSource.PixelHeight;
                var stride = width * ((bitmapSource.Format.BitsPerPixel + 7) / 8);
                var memoryBlockPointer = Marshal.AllocHGlobal(height * stride);
                bitmapSource.CopyPixels(new Int32Rect(0, 0, width, height), memoryBlockPointer, height * stride, stride);
                return new Bitmap(width, height, stride, PixelFormat.Format32bppPArgb, memoryBlockPointer);
            }
            float[,] data = new float[512, 512];
            for (int i = 0; i < 512; i++)
            {
                for (int j = 0; j < 512; j++)
                {
                    data[i, j] = bitmap.GetPixel(i, j).GetBrightness();
                }
            }
            zip.Generate(data);
            this.Cursor = null;
        }

        private void OpenZipButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog()
            {
                DefaultExt = ".zg",
                Filter = "Zipped Graphics (*.zg)|*.zg"
            };
            if (dialog.ShowDialog() != true)
            {
                return;
            }
            float[] data = Unzip.GetData(dialog.FileName);
            Bitmap bitmap = new Bitmap(512, 512, PixelFormat.Format32bppPArgb);
            for (int i = 0; i < 512 * 512; i++)
            {
                int blockIndex = i / 64;
                int bitIndex = i - blockIndex * 64;
                int xStart = blockIndex / 64 * 8;
                int yStart = blockIndex % 64 * 8;
                int brightness = Convert.ToInt32(data[i] * 256);
                if (brightness > 255)
                {
                    brightness = 255;
                }
                else if (brightness < 0)
                {
                    brightness = 0;
                }
                bitmap.SetPixel(xStart + bitIndex / 8, yStart + bitIndex % 8, Color.FromArgb(brightness, brightness, brightness));
            }

            BitmapImage ConvertToBitmapImage(Bitmap src)
            {
                MemoryStream memoryStream = new MemoryStream();
                src.Save(memoryStream, ImageFormat.Bmp);
                BitmapImage image = new BitmapImage();
                image.BeginInit();
                memoryStream.Seek(0, SeekOrigin.Begin);
                image.StreamSource = memoryStream;
                image.EndInit();
                return image;
            }
            this.ZippedImage.Source = ConvertToBitmapImage(bitmap);
            this.SaveImageButtun.IsEnabled = true;
        }

        private void SaveImageButtun_Click(object sender, RoutedEventArgs e)
        {
            BitmapSource source = this.ZippedImage.Source as BitmapSource;
            Bitmap bitmap = new Bitmap(source.PixelWidth, source.PixelHeight, PixelFormat.Format32bppPArgb);

            BitmapData data = bitmap.LockBits(new Rectangle(System.Drawing.Point.Empty, bitmap.Size), ImageLockMode.WriteOnly, System.Drawing.Imaging.PixelFormat.Format32bppPArgb);

            source.CopyPixels(Int32Rect.Empty, data.Scan0, data.Height * data.Stride, data.Stride);
            bitmap.UnlockBits(data);
            SaveFileDialog sfd = new SaveFileDialog()
            {
                DefaultExt = ".bmp",
                Filter = "Bitmap (*.bmp)|*.bmp"
            };
            if (sfd.ShowDialog() == true)
            {
                try
                {
                    bitmap.Save(sfd.FileName, ImageFormat.Bmp);
                }
                catch (Exception)
                {
                    MessageBox.Show("保存失败");
                }
            }
        }

    }
}
