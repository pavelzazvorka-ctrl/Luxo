using System;
using System.Text;
using System.Net.Http;
using System.Threading;
using System.CodeDom;
using System.Diagnostics;

namespace LuxoCmd
{
    internal class Program
    {
        public static int[] cs = new int[]{ 120, 90, 10, 90, 0 };
        public static float[] sd = new float[] { 0, 0, 0, 0, 0 };

        const string L1 = "10.0.0.84";
        const string L2 = "10.0.0.95";
        const string R = "10.0.0.158";

        static void servo(string addr, int servo, int angle)
        {
            using (var client = new HttpClient())
            {
                string url = $"http://{addr}/servo/{(1+servo)}/";
                string payload = $"angle={angle}";
                var res = client.PostAsync(url, new StringContent(payload, Encoding.UTF8, "application/x-www-form-urlencoded")
                );

                try
                {
                    res.Result.EnsureSuccessStatusCode();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.ToString());
                }
            }
        }

        static void Color(string addr, string color)
        {
            using (var client = new HttpClient())
            {
                string url = $"http://{addr}/color/0";
                string payload = $"RGB=#{color}";
                var res = client.PostAsync(url, new StringContent(payload, Encoding.UTF8, "application/x-www-form-urlencoded")
                );

                try
                {
                    res.Result.EnsureSuccessStatusCode();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.ToString());
                }
            }
        }

        static void Light(string addr, int light)
        {
            using (var client = new HttpClient())
            {
                string url = $"http://{addr}/light/0";
                string payload = $"light={light}";
                var res = client.PostAsync(url, new StringContent(payload, Encoding.UTF8, "application/x-www-form-urlencoded")
                );

                try
                {
                    res.Result.EnsureSuccessStatusCode();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.ToString());
                }
            }
        }

        static void light(string addr, int light)
        {
            using (var client = new HttpClient())
            {
                string url = $"http://{addr}/light/";
                string payload = $"light={light}";
                var res = client.PostAsync(url, new StringContent(payload, Encoding.UTF8, "application/x-www-form-urlencoded")
                );

                try
                {
                    res.Result.EnsureSuccessStatusCode();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.ToString());
                }
            }
        }

        static bool isServoReady(string addr)
        {
            using (var client = new HttpClient())
            {
                string url = $"http://{addr}/status/";
                string payload = "angle=0";
                var res = client.PostAsync(url, new StringContent(payload, Encoding.UTF8, "application/x-www-form-urlencoded")
                );

                try
                {
                    if (res.Result.StatusCode == System.Net.HttpStatusCode.OK) return true;
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.ToString());
                }
                return false;
            }
        }

        static bool waitForServo(string addr)
        {
            for(int i = 0;i<100;i++)
            {
                if (isServoReady(addr))
                {
                    return true;
                }
                Thread.Sleep(100);
            }
            return false;
        }

        static int[] last;

        static void move(string addr,int s1, int s2, int s3, int s4, int s5)
        {
            int[] ns = new int[5] { s1, s2, s3, s4, s5 };
            for (int i = 0; i < 5; i++)
            {
                //if (last[i] != ns[i])
                {
                    servo(addr, i, ns[i]);
                }
            }
            last = ns;
            waitForServo(addr);
        }
        
        static void RandomMove(string[] args)
        {
            last = new int[5];
            Random _random = new Random();
            for (int x = 0; x < 100; x++)
            {
                int pause =  (int) _random.Next(1, 50);
                for (int w = 0; w < pause; w++)
                {
                    Console.WriteLine(pause-w);
                    Thread.Sleep(1000);
                }

                light(L1,1);
                Console.WriteLine("*1");
                move(L1,90, 95, 94, 55, 90);
                Console.WriteLine("*2");
                move(L1, 0, 95, 38, 80, 34);
                Console.WriteLine("*3");
                move(L1, 180, 98, 78, 113, 178);
                Console.WriteLine("*4");
                move(L1, 180, 95, 119, 113, 14);
                Console.WriteLine("*5");

                // retract / relieve
                move(L1, 90, 95, 94, 55, 90);
                Console.WriteLine("*6");
                light(L1, 0);


            }
            Console.WriteLine("done");            
        }

        public static Stopwatch stopWatch;
        public static int bpm = 696;

        static void w(int wb)
        {
            int w = wb * 697;
            while (stopWatch.Elapsed.TotalMilliseconds < w)
            {
                Thread.Sleep(10);
            }
        }

        static int a()
        {
            int w = (int)(stopWatch.Elapsed.TotalMilliseconds / bpm);
            System.Console.WriteLine("Elps :" + w);
            return w;
        }

        static void s()
        {
            int c = (int)(stopWatch.Elapsed.TotalMilliseconds / bpm);
            c = (c + 1) * bpm;

            while (stopWatch.Elapsed.TotalMilliseconds < c)
            {
                Thread.Sleep(0);
            }
        }

        static void Main(string[] args)
        {
            // rocket

            stopWatch = new Stopwatch();
            stopWatch.Start();

            move(L1, 126, 96, 114, 45, 45);
            move(L2, 93, 44, 162, 46, 84);
            light(L1, 0);
            light(L2, 0);
            w(15);
            Color(R, "ffffff");
            w(2);
            Color(R, "00FF00");
            w(10);
            Color(R, "ff00FF");
            w(2);
            Color(R, "000000");
            w(10);
            Light(R, 4);
            w(10);
            Light(R, 5);
            light(L1, 1);
            light(L2, 1);
            move(L1, 58, 96, 84, 97, 115);
            move(L2, 11, 73, 82, 86, 11);
            w(10);
            Light(R, 0);
            w(140);
            move(L1, 126, 96, 114, 45, 45);
            move(L2, 93, 44, 162, 46, 84);
            light(L1, 0);
            light(L2, 0);

            Console.WriteLine("done");
        }


        static void Main2(string[] args)
        {
            // Bitter sweet symphony 86 / 171 BPM = 697 msec

            stopWatch = new Stopwatch();
            stopWatch.Start();
            

            last = new int[5];
            light(L1,1);
            light(L2, 1);
            move(L1, 90, 95, 94, 4, 90);
            move(L2, 90, 94, 94, 3, 89);
            light(L1, 0);
            light(L2, 0);

            // turn down
            move(L1, 100, 95, 133, 46, 90);

            a();
            w(25);
            a();

            int n = a();
            for (int x = 0; a() < 55; x+=2)
            {
                w(n + x );
                light(L1, 1);
                w(n + (x+1) );
                light(L1, 0);
                a();

                if (x > 8)
                {
                    if ((x % 4) == 0)
                    {
                        light(L2, 1);
                        Thread.Sleep(697/2);
                        light(L2, 0);
                    }
                }

            }
            Console.WriteLine("*1");
            a();

            move(L2, 120, 55, 150, 4, 84);
            move(L2, 146, 55, 150, 180, 180);
            light(L2, 1);

            w(60);

            // stand up
            light(L1, 1);
            move(L1, 90, 97, 60, 113, 90);
            for (int x = 0; x < 4; x++)
            {
                Console.WriteLine($"s {x}");
                s();
                move(L1, 72, 97, 60 + x * 10, 113, 72);
                s();
                move(L1, 108, 97, 60 + x * 10 + 5, 113, 108);
            }
            light(L1, 0);

            s();
            move(L1, 0, 97, 60, 113, 72);
            s();
            move(L1, 180, 97, 60, 113, 108);

            // L2 up
            move(L2, 146, 55, 150, 180, 180);

            // down 
            for (int x = 0; x < 4; x++)
            {
                Console.WriteLine($"down {x}");

                s();
                move(L1, 82, 98, 114 - x * 5, 96, 90);
                s();
                move(L1, 98, 101, 114-x*5+3, 104, 90);
            }

            // L2 down
            s();
            move(L2, 146, 55, 150, 180, 180);


            //show
            light(L1, 1);
            s();
            move(L1, 0, 95, 60, 113, 2);
            s();
            move(L1, 180, 95, 60, 113, 180);
            s();
            move(L1, 0, 95, 90, 113, 2);
            s();
            move(L1, 180, 95, 90, 113, 180);
            light(L1, 0);

            // L2 up
            move(L2, 146, 55, 150, 180, 180);

            // down 
            for (int x = 0; x < 4; x++)
            {
                Console.WriteLine($"down {x}");

                s();
                move(L1, 82, 98, 114 - x * 5, 96, 90);
                s();
                move(L1, 98, 101, 114 - x * 5 + 3, 104, 90);

            }

            //show
            light(L1, 1);
            s();
            move(L1, 0, 95, 60, 113, 180);
            move(L1, 180, 95, 60, 113, 2);
            move(L1, 0, 95, 90, 113, 180);
            move(L1, 180, 95, 90, 113, 2);
            light(L1, 0);

            // ignore
            move(L2, 120, 55, 150, 4, 84);
            move(L2, 146, 55, 150, 180, 180);
            light(L2, 1);
            Thread.Sleep(1000);
            light(L2, 0);

            // retract / relieve
            move(L1, 90, 95, 94, 55, 90);
            move(L2, 90, 90, 90, 5, 90);

            light(L1, 1);
            Thread.Sleep(100);
            light(L1, 0);
            Thread.Sleep(100);
            light(L2, 1);
            Thread.Sleep(100);
            light(L2, 0);
            Thread.Sleep(100);
            light(L1, 1);
            light(L2, 1);
            Thread.Sleep(100);
            light(L1, 0);
            light(L2, 0);

            Console.WriteLine("done");
        }

        /*
        static void go(int i,bool back=true)
        {
            switch (i)
            {
                case 0:
                    move(0, 86, 85, 77, 93);
                    move(0, 86, 90, 80, 90);
                    break;
                case 5:
                    move(5,51,170,104,0);
                    break;
                case 4:
                    move(5, 124, 170, 104, 44);
                    break;
                case 3:
                    move(43,89,170,44,123);
                    break;
                case 2:
                    move(12, 87, 170, 108, 180);
                    break;
                case 1:
                    move(53,95,170,140,180);                    
                    break;
             }
            if (back)
            {
                Thread.Sleep(200);
                move(0, 86, 85, 77, 93);
                move(0, 86, 90, 80, 90);
            }
        }

        static void Main(string[] args)
        {
            Thread.Sleep(500);
            go(5);
            Thread.Sleep(1100);
            go(3);
            Thread.Sleep(100);
            go(4);
            Thread.Sleep(200);
            go(1);
            Thread.Sleep(100);
            go(2);
            Console.WriteLine("done");
        }*/

    }
}
