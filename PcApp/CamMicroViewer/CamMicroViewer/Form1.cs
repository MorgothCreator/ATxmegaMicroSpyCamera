using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CamMicroViewer
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        string data_in = "";
        string frame_data = "";
        Bitmap bmp;
        int char_per_pixel = 2;
        int count = 0;
        int old_avarage = 0;

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            data_in += serialPort1.ReadExisting();
            int index_of_line = data_in.IndexOf("\n\r");
            if (index_of_line != -1)
            {
                int H_Cnt = 0;
                int V_Cnt = 0;
                int avarage_level = Convert.ToInt32(data_in.Substring(index_of_line - 2, 2), 16);
                Graphics g = Graphics.FromImage(pictureBox1.Image);
                {
                    //g.Clear(Color.White);
                    //if (old_avarage - 0 > avarage_level || old_avarage + 0 < avarage_level)
                   // {
                        old_avarage = avarage_level;
                        try
                        {
                            frame_data = data_in.Substring(0, index_of_line - 1);
                            while (count != frame_data.Length)
                            {
                                string tmp = frame_data.Substring(count * char_per_pixel, char_per_pixel);
                                if (tmp == "\r\n")
                                {
                                    //label1.Text = "" + H_Cnt;
                                    H_Cnt = 0;
                                    V_Cnt++;
                                    count++;
                                    continue;
                                }
                                if (tmp == "\n\r")
                                {
                                    break;
                                }
                                if (char_per_pixel == 2)
                                {
                                    //if (Convert.ToInt32(tmp, 16) < 0x7F || Convert.ToInt32(tmp, 16) > 0x81)
                                    //{
                                    Color clr = Color.FromArgb(Convert.ToInt32(tmp, 16), Convert.ToInt32(tmp, 16), Convert.ToInt32(tmp, 16));
                                    g.DrawRectangle(new Pen(clr), (H_Cnt * 4), (V_Cnt * 4), 1, 1);
                                    g.DrawRectangle(new Pen(clr), (H_Cnt * 4) + 1, (V_Cnt * 4), 1, 1);
                                    g.DrawRectangle(new Pen(clr), (H_Cnt * 4), (V_Cnt * 4) + 1, 1, 1);
                                    g.DrawRectangle(new Pen(clr), (H_Cnt * 4) + 1, (V_Cnt * 4) + 1, 1, 1);

                                    g.DrawRectangle(new Pen(clr), (H_Cnt * 4) + 2, (V_Cnt * 4) + 0, 1, 1);
                                    g.DrawRectangle(new Pen(clr), (H_Cnt * 4) + 2, (V_Cnt * 4) + 1, 1, 1);
                                    g.DrawRectangle(new Pen(clr), (H_Cnt * 4) + 2, (V_Cnt * 4) + 2, 1, 1);
                                    g.DrawRectangle(new Pen(clr), (H_Cnt * 4) + 1, (V_Cnt * 4) + 2, 1, 1);
                                    g.DrawRectangle(new Pen(clr), (H_Cnt * 4) + 0, (V_Cnt * 4) + 2, 1, 1);
                                    //}
                                }
                                H_Cnt++;
                                count++;
                            }
                        }
                        catch { };
                    //}
                    //label2.Text = "" + V_Cnt;
                    g.DrawString("Avarage:" + avarage_level, new Font("Arial", 8), Brushes.White, new PointF(0, 0));
                    data_in = data_in.Substring(index_of_line + 2);
                    count = 0;
                    pictureBox1.Invalidate();
                }
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            bmp = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            using (Graphics g = Graphics.FromImage(bmp))
            {
                g.Clear(Color.White);
            }
            pictureBox1.Image = bmp;
            serialPort1.Open();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            serialPort1.Close();
        }
    }
}
