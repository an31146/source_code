using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace FourierSeries
{
    public class FourierSeries : Form
    {
        private BufferedGraphicsContext context;
        private BufferedGraphics grafx;

        public FourierSeries()
            : base()
        {
            // Configure the Form for this example.
            this.Text = "Fourier Series approximation";
            this.Resize += new EventHandler(this.OnResize);
            this.SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint, true);

            // Retrieves the BufferedGraphicsContext for the 
            // current application domain.
            context = BufferedGraphicsManager.Current;

            this.InitializeComponent();
            // Sets the maximum size for the primary graphics buffer
            // of the buffered graphics context for the application
            // domain.  Any allocation requests for a buffer larger 
            // than this will create a temporary buffered graphics 
            // context to host the graphics buffer.
            context.MaximumBuffer = new Size(this.Width + 1, this.Height + 1);

            // Allocates a graphics buffer the size of this form
            // using the pixel format of the Graphics created by 
            // the Form.CreateGraphics() method, which returns a 
            // Graphics object that matches the pixel format of the form.
            grafx = context.Allocate(this.CreateGraphics(),
                 new Rectangle(0, 0, this.Width, this.Height));

            // Draw the first frame to the buffer.
            DrawToBuffer(grafx.Graphics);
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // FourierSeries
            // 
            this.ClientSize = new System.Drawing.Size(1014, 564);
            this.Name = "FourierSeries";
            this.ResumeLayout(false);

        }

        private void OnResize(object sender, EventArgs e)
        {
            // Re-create the graphics buffer for a new window size.
            context.MaximumBuffer = new Size(this.Width + 1, this.Height + 1);
            if (grafx != null)
            {
                grafx.Dispose();
                grafx = null;
            }
            grafx = context.Allocate(this.CreateGraphics(),
                new Rectangle(0, 0, this.Width, this.Height));

            // Cause the background to be cleared and redraw.
            grafx.Graphics.FillRectangle(Brushes.Black, 0, 0, this.Width, this.Height);
            DrawToBuffer(grafx.Graphics);
            this.Refresh();
        }

        // Fourier series for a square wave
        private double f(double x)
        {
            double ret = 0.0, t;    // abs = 1.0;

            // upper limit for t is the number of harmonics
            for (t = 1.0; t <= 13.0; t += 2.0)
            {
                ret += Math.Sin(t * x) / t;
            }

            return (4.0 / Math.PI * ret);
        }

        // Fourier series for a sawtooth wave
        private double s(double x)
        {
            double ret = 0.0, t;

            // upper limit for t is the number of harmonics
            for (t = 1.0; t <= 31.0; t++)
            {
                ret += Math.Sin(2.0 * t * x) / t;
            }

            return (0.5 - ret / Math.PI);
        }

        // Fourier series for a triangle wave
        private double t(double x)
        {
            double ret = 0.0, t, abs = 1.0;

            for (t = 1.0; t <= 7.0; t += 2.0)
            {
                ret += abs * Math.Sin(t * x) / (t * t);
                abs = -abs;
            }

            return (8.0 / (Math.PI * Math.PI) * ret);
        }
        
        private double Square(double x)
        {
            return ( Math.Sign(Math.Sin(x)) );
        }

        private double Heaviside(double x)
        {
            return ( x>0 ? 1 : 0 );
        }

        private double Sawtooth(double x)
        {
            return ( 4.0 / Math.PI * (x % Math.PI/4) * Heaviside(x) );
        }

        private double Triangle(double x)
        {
            return (2.0 / Math.PI * Math.Asin(Math.Sin(x)));
        }

        private void DrawToBuffer(Graphics g)
        {
            // Plot the graph given by the function f(x).
            Point[] pt = (Point[])Array.CreateInstance(typeof(Point), this.Width);

            pt.Initialize();
            for (int i = 0; i < this.Width; i++)
            {
                int px = i;

                int offset = (int)(this.Height / 2.5 * s(i * 4.0 * Math.PI / (double)this.Width) );
                int py = this.Height / 2 - offset - 20;
                pt.SetValue(new Point(px, py), i);
            }
            g.DrawLines(new Pen(Color.Red, 2.0f), pt);

            pt.Initialize();
            for (int i = 0; i < this.Width; i++)
            {
                int px = i;

                int offset = (int)(this.Height / 2.5 * Sawtooth(i * 4.0 * Math.PI / (double)this.Width));
                int py = this.Height / 2 - offset - 20;
                pt.SetValue(new Point(px, py), i);
            }
            g.DrawLines(new Pen(Color.White, 2.0f), pt);

            /*pt = (Point[])Array.CreateInstance(typeof(Point), 4);
            pt.Initialize();

            pt.SetValue(new Point(100, 100), 0);
            pt.SetValue(new Point(300, 240), 0);
            pt.SetValue(new Point(500, 60), 0);
            pt.SetValue(new Point(100, 400), 0);

            g.DrawBezier(new Pen(Color.Blue, 1), new Point(100, 100), new Point(300, 40), 
                                                 new Point(500, 60),  new Point(100, 400));
            */
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            grafx.Render(e.Graphics);
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.Run(new FourierSeries());
        }
    }
}