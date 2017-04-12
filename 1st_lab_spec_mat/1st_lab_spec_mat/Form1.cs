using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Numerics;
using System.IO;

namespace _1st_lab_spec_mat
{
    public partial class Form1 : Form
    {
        List<AlgIteration> iter;
        BigInteger x;
        BigInteger y;

        public Form1()
        {
            InitializeComponent();
            iter = new List<AlgIteration>();
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            string time=null;
            DateTime st = DateTime.UtcNow;
            richTextBox3.Text = Algoritms.Rash_evkl(BigInteger.Parse(richTextBox1.Text), BigInteger.Parse(richTextBox2.Text), iter, out x, out y, out time).ToString();
            richTextBox4.Text = x.ToString();
            richTextBox5.Text = y.ToString();

            DateTime end = DateTime.UtcNow;
            using (StreamWriter t = new StreamWriter("12.txt"))
            {
                int m = 1;
                foreach (AlgIteration i in iter)
                {
                    
                    string it = "n =" + m.ToString()+ "  d= " + i.Dprop.ToString() + "  x= " + i.Xprop.ToString() + "  y= " + i.Yprop.ToString();
                    t.WriteLine(it);
                    m++;
                }
                t.WriteLine((st - end).ToString());

            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            string time = null;
            DateTime st = DateTime.UtcNow;
            richTextBox3.Text = Algoritms.Rash_evkl_usech_ostatk(BigInteger.Parse(richTextBox1.Text), BigInteger.Parse(richTextBox2.Text), iter, out x, out y, out time).ToString();
            DateTime end = DateTime.UtcNow;
            richTextBox4.Text = x.ToString();
            richTextBox5.Text = y.ToString();
            using (StreamWriter t = new StreamWriter("RASH_EVKL.txt"))
            {
                int m = 1;
                foreach (AlgIteration i in iter)
                {

                    string it = "n =" + m.ToString() + "  d= " + i.Dprop.ToString() + "  x= " + i.Xprop.ToString() + "  y= " + i.Yprop.ToString();
                    t.WriteLine(it);
                    m++;
                }
                t.WriteLine((st-end).ToString());
            }

        }

        private void button3_Click(object sender, EventArgs e)
        {
            string time = null;
            DateTime st = DateTime.UtcNow;
            richTextBox3.Text = Algoritms.Rash_evkl_Bin(BigInteger.Parse(richTextBox1.Text), BigInteger.Parse(richTextBox2.Text), iter, out x, out y, out time).ToString();
            DateTime end = DateTime.UtcNow;
            richTextBox4.Text = x.ToString();
            richTextBox5.Text = y.ToString();
            using (StreamWriter t = new StreamWriter("EVKL_BIN.txt"))
            {
                int m = 1;
                foreach (AlgIteration i in iter)
                {

                    string it = "n =" + m.ToString() + "  d= " + i.Dprop.ToString() + "  x= " + i.Xprop.ToString() + "  y= " + i.Yprop.ToString();
                    t.WriteLine(it);
                    m++;
                }
                t.WriteLine((st - end).ToString());
            }

        }
    }

    public class AlgIteration
    {
        public BigInteger Dprop { get; set; }
        public BigInteger Xprop { get; set; }
        public BigInteger Yprop { get; set; }



        public AlgIteration (BigInteger d, BigInteger x, BigInteger y)
        {
            Dprop = d;
            Xprop = x;
            Yprop = y;
        }

    }

}
