using System;
using System.Drawing;
using System.IO;

namespace bmp_to_matrix
{
    class Program
    {
        static void Main(string[] args)
        {
            
            foreach (var f in Directory.GetFiles(AppDomain.CurrentDomain.BaseDirectory + "/symbols", "*.bmp"))
            {
                Bitmap bmp = new Bitmap(f);
                String outStr = "byte indx[][2] ={{";
                int counter = 0;
                for (int y = 0; y < bmp.Height; y ++)
                {
                    for (int x = 0; x < bmp.Width; x++)
                    {
                        if (bmp.GetPixel(bmp.Width - x - 1, bmp.Height - y -1).R >= 1)
                        {
                            outStr += x.ToString() + ", " + y.ToString() + "},{";
                            counter ++;
                        }
                    }
                }

                outStr = outStr.Substring(0, outStr.Length - 2) + "};\nsendTextToMatrix(indx, "+ counter.ToString() + ");";
                File.WriteAllText(f.Replace(".bmp", ".txt"), outStr);
            }
            
        }
    }
}
