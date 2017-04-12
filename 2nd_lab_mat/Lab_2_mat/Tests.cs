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
    public class Tests
    {

        BigInteger Testing;

        public Tests(BigInteger test)
        {
            Testing = test;
        }

        public bool FermaTest(Form1 form)
        {
            string result = "Ferma start \n";
            RNGCryptoServiceProvider Rng = new RNGCryptoServiceProvider();
            byte[] randnumber = new byte[(Testing.ToByteArray().Length > 1 ? (Testing.ToByteArray().Length - 1) : 1)];
            bool isok = true;

            DateTimeOffset start = DateTimeOffset.Now;

            for (int i = 0; i < 5; i++)
            {
                Rng.GetBytes(randnumber);
                BigInteger a = BigInteger.Abs(new BigInteger(randnumber));

                if (BigInteger.ModPow(a, (Testing - BigInteger.One), Testing) != BigInteger.One)
                {
                    isok = false;
                    result += i.ToString() + "____" + a.ToString() + "NOT OK \n";
                    break;
                }
                result += i.ToString() + "____" + a.ToString() + " Is Ok \n";
            }
            result += "Ferma Time Ticks_" + (DateTimeOffset.Now - start).Ticks.ToString() + "\n";
            form.ResultBox.Text += result;
            return isok;
        }

        public bool SolovTest(Form1 form)
        {
            string result = "\nSolov start \n";
            RNGCryptoServiceProvider Rng = new RNGCryptoServiceProvider();
            byte[] randnumber = new byte[(Testing.ToByteArray().Length > 1 ? (Testing.ToByteArray().Length - 1) : 1)];
            bool isok = true;

            DateTimeOffset start = DateTimeOffset.Now;

            for (int i = 0; i < 5; i++)
            {
                Rng.GetBytes(randnumber);
                BigInteger a = BigInteger.Abs(new BigInteger(randnumber));
                BigInteger r = BigInteger.ModPow (a, (Testing - BigInteger.One) / 2, Testing);

                if ((r != BigInteger.One) && (r != (Testing - BigInteger.One)))
                {
                    isok = false;
                    result += i.ToString() + "____" + a.ToString() + "NOT OK \n";
                    break;
                }
                BigInteger s = Jacob(a, Testing);

                if ((r == s  /* Если r сравнимо с s по модулю n(testing) и равно единице*/) && ((r == Testing - 1) && (s == -1)/* Если r сравнимо с s по модулю n(testing) и r=n-1 а s=-1 */ ))
                {
                    isok = false;
                    result += i.ToString() + "____" + a.ToString() + "NOT OK   Jacobi = "+s.ToString()+" \n";
                    break;
                }

                    result += i.ToString() + "____" + a.ToString() + " Is Ok Jacobi = " + s.ToString() + " \n";
            }

            result += "Solov Time Ticks_" + (DateTimeOffset.Now - start).Ticks.ToString() + "\n";
            form.ResultBox.Text += result;
            return isok;

        }

        public bool RabinTest(Form1 form)
        {
            string result = "\nRabin Test start \n";
            RNGCryptoServiceProvider Rng = new RNGCryptoServiceProvider();
            byte[] randnumber = new byte[(Testing.ToByteArray().Length > 1 ? (Testing.ToByteArray().Length - 1) : 1)];
            bool isok = true;

            DateTimeOffset start = DateTimeOffset.Now;

            BigInteger r = Testing - 1; //n-1
            BigInteger s = 0;

            do
            {
                r/= 2;
                s++;
            } while (r % 2 == 0); //пока r делимо на 2  пункт1



            for (int i = 0; i < 5; i++)
            {
                Rng.GetBytes(randnumber);
                BigInteger a = BigInteger.Abs(new BigInteger(randnumber));//2 pynkt
                BigInteger y = BigInteger.ModPow(a, r, Testing);//3 pynkt

                if ((y != 1) && (y != Testing - 1)) //4 pynkt
                {
                    BigInteger j = 1; //4.1
                    while ((j <= (s - 1)) && (y != (Testing - 1))) //4.2
                    {
                        y = BigInteger.ModPow(y, 2, Testing);//4.2.1
                        if (y == 1)//4.2.2
                        {
                            isok = false;
                            result += i.ToString() + "____" + a.ToString() + "NOT OK \n";
                            form.ResultBox.Text += result;
                            return isok;
                        }
                        j = j + 1; //4.2.3
                    }
                    if (y != (Testing - 1)) //4.3
                    {
                        isok = false;
                        result += i.ToString() + "____" + a.ToString() + "NOT OK \n";
                        form.ResultBox.Text += result;
                        return isok;
                    }
                }
                result += i.ToString() + "____" + a.ToString() + " Is Ok \n";
            }

            result += "Rabin Time Ticks_" + (DateTimeOffset.Now - start).Ticks.ToString() + "\n";
            form.ResultBox.Text += result;
            return isok;
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

        private BigInteger Jacob(BigInteger a, BigInteger b)
        {
            if (NOD(a, b) != 1)
                return 0;
            else
            {
                BigInteger r = 1;
                if (a < 0)
                {
                    a = -a;
                    if (b % 4 == 3)
                        r = -r;
                }
                do
                {
                    BigInteger t = 0;
                    while (a % 2 == 0)
                    {
                        t += 1;
                        a /= 2;
                    }
                    if (t % 2 != 0 && (b % 8 == 3 || b % 8 == 5))
                        r = -r;
                    if (a % 4 == 3 && b % 4 == 3)
                        r = -r;
                    BigInteger c = a;
                    a = b % c;
                    b = c;
                }
                while (a != 0);
                return r;
            }
        }
    }
}
