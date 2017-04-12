using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Numerics;
using System.Threading.Tasks;
using Lab_2_mat;
using System.Security.Cryptography;


namespace Lab_2_mat
{
    public class Mnozhit
    {
        BigInteger Testing;

        public Mnozhit(BigInteger test)
        {
            Testing = test;
        }

        public bool P0Pollard(Form1 form)
        {
            bool isok = true;
            string result = "Pollard-0 start \n";
            BigInteger c = 2;
            BigInteger a = c;//1
            BigInteger b = c;//1
            BigInteger d = 0;
            BigInteger t =1;//да пребудет t=1 потому-что топинамбур
            do
            {
                a = X2_minus_t(a, t);
                a = a % Testing;

                b = X2_minus_t(b, t);
                b = b % Testing;

                b = X2_minus_t(b, t);
                b = b % Testing;

                d = NOD(BigInteger.Abs(a - b), Testing);

                if(d==Testing)
                {
                    isok = false;
                    result += d.ToString() + "NOT OK d==testing a-b="+(a-b).ToString()  +" \n";
                    break;
                }


                if(( 1<d )&&(d<Testing))
                {
                    result += d.ToString() + " OK \n";
                }

            } while (d == 1);


            form.ResultBox.Text += result;
            return isok;
        }

        public bool P_1Pollard(Form1 form)
        {
            BigInteger t=101;
            BigInteger p=10;
            form.ResultBox.Text+= BigInteger.Log10(t).ToString();
            return false;
        }

        
        public BigInteger X2_minus_t(BigInteger x, BigInteger t)
        {
            return x*x - t;
        }

        private BigInteger NOD(BigInteger m, BigInteger n)
        {
            while (m != 0 && n != 0)
            {
                if (m >= n)
                    m %= n;
                else
                    n %= m;
            }
            return m + n;
        }


    }
}
