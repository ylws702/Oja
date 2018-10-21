using Microsoft.Win32;
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;
/// <summary>
/// Oja算法相关
/// </summary>
namespace Oja
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// 图片路径
        /// </summary>
        private string imagePath;
        /// <summary>
        /// 压缩图片路径
        /// </summary>
        private string zipPath;
        /// <summary>
        /// 构造函数
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();
        }
        /// <summary>
        /// 点击打开图片按钮
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OpenImageButtun_Click(object sender, RoutedEventArgs e)
        {
            //打开文件对话框
            OpenFileDialog dialog = new OpenFileDialog()
            {
                DefaultExt = ".bmp",
                Filter = "Bitmap (*.bmp)|*.bmp"
            };
            //如果取消操作,返回
            if (dialog.ShowDialog() != true)
            {
                return;
            }
            //获取打开的图片路径
            this.imagePath = dialog.FileName;
            try
            {
                //打开并显示图片
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
        /// <summary>
        /// 点击保存压缩文件按钮
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SaveZipButtun_Click(object sender, RoutedEventArgs e)
        {
            //保存文件对话框
            SaveFileDialog dialog = new SaveFileDialog()
            {
                FileName = Path.GetFileNameWithoutExtension(this.imagePath),
                DefaultExt = ".zg",
                Filter = "Zipped Graphics (*.zg)|*.zg"
            };
            //如果取消操作,返回
            if (dialog.ShowDialog() != true)
            {
                return;
            }
            //获取保存的压缩文件路径
            string zipPath = dialog.FileName;
            try
            {
                //新建压缩成员
                Zip zip = new Zip(zipPath);
                Bitmap bitmap = ConvertToBitmap(this.OriginImage.Source as BitmapSource);
                //图像大小不符合512*512,缩放图像至512*512
                if (bitmap.Width != 512 || bitmap.Height != 512)
                {
                    Bitmap m = bitmap;
                    bitmap = new Bitmap(m, 512, 512);
                    m.Dispose();
                }
                //图像数据数组
                float[,] data = new float[512, 512];
                for (int i = 0; i < 512; i++)
                {
                    for (int j = 0; j < 512; j++)
                    {
                        data[i, j] = bitmap.GetPixel(i, j).GetBrightness();
                    }
                }
                //生成压缩数据并保存
                zip.Generate(data);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Exception");
            }
        }
        /// <summary>
        /// 点击打开压缩文件按钮
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OpenZipButton_Click(object sender, RoutedEventArgs e)
        {
            //本地方法: 将Bitmap转换为BitmapImage
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
            //打开文件对话框
            OpenFileDialog dialog = new OpenFileDialog()
            {
                DefaultExt = ".zg",
                Filter = "Zipped Graphics (*.zg)|*.zg"
            };
            //如果取消操作,返回
            if (dialog.ShowDialog() != true)
            {
                return;
            }
            //获取打开的压缩文件路径
            this.zipPath = dialog.FileName;
            //获取压缩文件数据
            float[] data = Unzip.GetData(this.zipPath);
            //新建位图
            Bitmap bitmap = new Bitmap(512, 512, PixelFormat.Format32bppPArgb);
            //根据数据重绘图像
            for (int i = 0; i < 512 * 512; i++)
            {
                //计算点坐标
                int blockIndex = i / 64;
                int bitIndex = i - blockIndex * 64;
                int xStart = blockIndex / 64 * 8;
                int yStart = blockIndex % 64 * 8;
                int x = xStart + bitIndex / 8;
                int y = yStart + bitIndex % 8;
                //计算RGB值
                int brightness = Convert.ToInt32(data[i] * 256);
                //数据溢出处理
                if (brightness > 255)
                {
                    brightness = 255;
                }
                else if (brightness < 0)
                {
                    brightness = 0;
                }
                //灰度转RGB
                Color pixelColor = Color.FromArgb(brightness, brightness, brightness);
                //绘制像素点
                bitmap.SetPixel(x, y, pixelColor);
            }
            //显示图片
            this.ZippedImage.Source = ConvertToBitmapImage(bitmap);
            this.SaveImageButtun.IsEnabled = true;
        }

        private void SaveImageButtun_Click(object sender, RoutedEventArgs e)
        {
            
            Bitmap bitmap = ConvertToBitmap(this.ZippedImage.Source as BitmapSource);
            //保存文件对话框
            SaveFileDialog sfd = new SaveFileDialog()
            {
                FileName = Path.GetFileNameWithoutExtension(this.zipPath) + "_zipped",
                DefaultExt = ".bmp",
                Filter = "Bitmap (*.bmp)|*.bmp"
            };
            if (sfd.ShowDialog() == true)
            {
                try
                {
                    //保存图片
                    bitmap.Save(sfd.FileName, ImageFormat.Bmp);
                }
                catch (Exception)
                {
                    MessageBox.Show("保存失败");
                }
            }
        }
        /// <summary>
        /// 将BitmapSource转换为Bitmap
        /// </summary>
        /// <param name="bitmapSource">要转换的BitmapSource</param>
        /// <returns>转换出的Bitmap</returns>
        private static Bitmap ConvertToBitmap(BitmapSource bitmapSource)
        {
            var width = bitmapSource.PixelWidth;
            var height = bitmapSource.PixelHeight;
            var stride = width * ((bitmapSource.Format.BitsPerPixel + 7) / 8);
            var memoryBlockPointer = Marshal.AllocHGlobal(height * stride);
            bitmapSource.CopyPixels(
                new Int32Rect(0, 0, width, height),
                memoryBlockPointer,
                height * stride,
                stride);
            return new Bitmap(width, height, stride, PixelFormat.Format32bppPArgb, memoryBlockPointer);
        }
    }
}
