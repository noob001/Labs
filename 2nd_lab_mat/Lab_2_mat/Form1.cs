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
using Lab_2_mat;

namespace Lab_2_mat
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        public RichTextBox ResultBox
        {
            get { return richTextBox2; }
        }

        public bool BreakFlag
        {
            get { return breakflag; }
        }

        private bool breakflag;

        private void Form1_Load(object sender, EventArgs e)
        {
            breakflag = false;
        }

        private void Тест_Click(object sender, EventArgs e)
        {
            richTextBox2.Text = "";
            Tests IsPrime = new Tests(BigInteger.Parse(richTextBox1.Text));
            Ferma.Text = IsPrime.FermaTest(this) ? "OK":" NOT OK";
            Solov.Text = IsPrime.SolovTest(this) ? "OK" : " NOT OK";
            RabMil.Text = IsPrime.RabinTest(this) ? "OK" : " NOT OK";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            richTextBox2.Text = "";
            breakflag = false;
            Mnozhit razloj = new Mnozhit (BigInteger.Parse(richTextBox1.Text));
            razloj.P0Pollard(this);

        }

        private void button2_Click(object sender, EventArgs e)
        {
            breakflag = true;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            richTextBox2.Text = "";
            breakflag = false;
            Mnozhit razloj = new Mnozhit(BigInteger.Parse(richTextBox1.Text));
            razloj.P_1Pollard(this);
        }
    }
}
