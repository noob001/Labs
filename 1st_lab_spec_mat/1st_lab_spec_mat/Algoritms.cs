using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Numerics;
using System.Threading;
using _1st_lab_spec_mat;

namespace _1st_lab_spec_mat
{
    public static class Algoritms
    {

        public static BigInteger Rash_evkl (BigInteger ta,BigInteger ib, List<AlgIteration> ItList, out BigInteger x, out BigInteger y, out string time)
        {
            DateTimeOffset st = DateTimeOffset.Now;
            DateTimeOffset end;
            BigInteger md = 0;
            if (ta<ib)
            {
                md = ta;
                ta = ib;
                ib = md;
                md = 0;
            }

            if (ib <= 0)
            {
                md = ta;
                x = 1;
                y = 0;
                
                end = DateTimeOffset.Now;
                time = (end - st).ToString();
                return md;
            }

            BigInteger x1 = 0;
            BigInteger x2 = 1;
            BigInteger y1 = 1;
            BigInteger y2 = 0;           
            BigInteger kq = 0;
            BigInteger or = 0;

            while (ib>0)
            {
                kq = ta / ib;
                or = ta - kq * ib;
                x = x2 - kq * x1;
                y = y2 - kq * y1;
                ta = ib;
                ib = or;
                x2 = x1;
                y2 = y1;
                x1 = x;
                y1 = y;

                if (ItList.Count<20)
                {
                    ItList.Add(new AlgIteration(ta, x2, y2));
                }
                else
                {
                    ItList.RemoveAt(14); //удаляем 15 ый элемент, записываем новый.
                    ItList.Add(new AlgIteration(ta, x2, y2));
                }



            }
            md = ta;
            x = x2;
            y = y2;
            end = DateTimeOffset.Now;
            time = (end - st).Ticks.ToString();
            return md; 
        }

        public static BigInteger Rash_evkl_usech_ostatk(BigInteger a, BigInteger b, List<AlgIteration> ItList, out BigInteger x, out BigInteger y, out string time)
        {
            DateTimeOffset st = DateTimeOffset.Now;
            DateTimeOffset end;
            BigInteger d = 0;
            if (a < b)
            {
                d = a;
                a = b;
                b = d;
                d = 0;
            }

            BigInteger r0 = a;
            BigInteger r1 = b;

            BigInteger x0 = 1;
            BigInteger x1 = 0;
            BigInteger y0 = 0;
            BigInteger y1 = 1;

            BigInteger x2, y2, q, r2;


            while (true)
            {

                q = BigInteger.DivRem(r0, r1,out r2);

                if (r2==0)
                {
                    d = r1;
                    x = x1;
                    y = y1;
                    
                    end = DateTimeOffset.Now;
                    time = (end - st).Ticks.ToString();
                    return d;
                }
                x2 = x0 - q * x1;
                y2 = y0 - q * y1;

                if (ItList.Count < 20)
                {
                    ItList.Add(new AlgIteration(r2, x2, y2));
                }
                else
                {
                    ItList.RemoveAt(14); //удаляем 15 ый элемент, записываем новый.
                    ItList.Add(new AlgIteration(r2, x2, y2));
                }

                x0 = x1;
                x1 = x2;
                y0 = y1;
                y1 = y2;
                r0 = r1;
                r1 = r2;

                if(r2>BigInteger.Abs(r0/2))
                {
                    r2 = r2- r1;
                    x2 = x2-x1;
                    y2 = y2-y1;
                }


            }
        }

        public static BigInteger Rash_evkl_Bin(BigInteger a, BigInteger b, List<AlgIteration> ItList, out BigInteger x, out BigInteger y, out string time)
        {
            DateTimeOffset st = DateTimeOffset.Now;
            DateTimeOffset end;
            BigInteger g = 1;

            while ((a%2==0)&&(b%2==0))
            {
                a = a / 2;
                b = b / 2;
                g = g * 2;
            }
            BigInteger u = a;
            BigInteger v = b;

            BigInteger A = 1;
            BigInteger B = 0;
            BigInteger C = 0;
            BigInteger D = 1;

            while (true)
            {
                while (u % 2 == 0)
                {
                    u = u / 2;
                    if ((A % 2 == 0) && (B % 2 == 0))
                    {
                        A = A / 2;
                        B = B / 2;
                    }
                    else
                    {
                        A = (A + b) / 2;
                        B = (B - a) / 2;
                    }
                }

                while (v % 2 == 0)
                {
                    v = v / 2;
                    if ((C % 2 == 0) && (D % 2 == 0))
                    {
                        C = C / 2;
                        D = D / 2;
                    }
                    else
                    {
                        C = (C + b) / 2;
                        D = (D - a) / 2;
                    }
                }

                if (u >= v)
                {
                    u = u - v;
                    A = A - C;
                    B = B - D;

                    if (ItList.Count < 20)
                    {
                        ItList.Add(new AlgIteration(u, A, B));
                    }
                    else
                    {
                        ItList.RemoveAt(14); //удаляем 15 ый элемент, записываем новый.
                        ItList.Add(new AlgIteration(u, A, B));
                    }
                }
                else
                {
                    v = v - u;
                    C = C - A;
                    D = D - B;

                    if (ItList.Count < 20)
                    {
                        ItList.Add(new AlgIteration(v, C, D));
                    }
                    else
                    {
                        ItList.RemoveAt(14); //удаляем 15 ый элемент, записываем новый.
                        ItList.Add(new AlgIteration(v, C, D));
                    }


                }

                if (u == 0)
                {
                    BigInteger d = g * v;
                    x = C;
                    y = D;
                    
                    end = DateTimeOffset.Now;
                    time = (DateTimeOffset.Now - st).Ticks.ToString();
                    return d;
                }
            }
        }
    }
}
