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
        private Timer timer1;
		private byte bufferingMode;
		private int x;
		private double w;
		Point[] pt = (Point[])Array.CreateInstance(typeof(Point), 4);

        public FourierSeries()
            : base()
        {
            // Configure the Form for this example.
            this.Text = "Fourier Series approximation";
            this.Resize += new EventHandler(this.OnResize);
            this.SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint, true);
			this.SetStyle(ControlStyles.OptimizedDoubleBuffer, false);

			// Configure a timer to draw graphics updates.
            timer1 = new Timer();
            timer1.Interval = 20;
            timer1.Tick += new EventHandler(this.OnTimer);
			timer1.Start();

            pt.Initialize();
			pt.SetValue(new Point(600, 100), 0);
            pt.SetValue(new Point(300, 240), 1);
            pt.SetValue(new Point(500, 60), 2);
            pt.SetValue(new Point(100, 400), 3);

            bufferingMode = 2;
			x = 0;
			w = 0.0;
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
            for (t = 1.0; t <= 15.0; t++)
            {
                ret += Math.Sin(2.0 * t * x) / t;
            }

            return (0.0 - ret / Math.PI);
        }

        // Fourier series for a triangle wave
        private double t(double x)
        {
            double ret = 0.0, t, abs = 1.0;

            for (t = 1.0; t <= 3.0; t += 2.0)
            {
                ret += abs * Math.Sin(t * x) / (t * t);
                abs = -abs;
            }

            return (6.0 / (Math.PI * Math.PI) * ret);
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
            return ( -0.5 + 4.0/Math.PI * (x % Math.PI/4) * Heaviside(x) );
        }

        private double Triangle(double x)
        {
            return (1.5 / Math.PI * Math.Asin(Math.Sin(x)));
        }

        private void DrawToBuffer(Graphics g)
        {
			// Clear background
            g.FillRectangle(Brushes.Black, 0, 0, this.Width, this.Height);
			
			// Plot the graph given by the function f(x).
            Point[] pt1 = (Point[])Array.CreateInstance(typeof(Point), this.Width);
			Point[] pt2 = (Point[])Array.CreateInstance(typeof(Point), this.Width);
			
            pt1.Initialize();
            pt2.Initialize();
            
			for (int i = 0; i < this.Width; i++)
            {
                int px = (i + x) % this.Width;
				int offset = (int)(this.Height / 2.5 * Triangle(i * 4.0 * Math.PI / (double)this.Width));
				int py = this.Height / 2 - offset - 20;
                
				pt1.SetValue(new Point(px, py), px);

                offset = (int)(this.Height / 2.5 * t(i * 4.0 * Math.PI / (double)this.Width) );
                py = this.Height / 2 - offset - 20;
				pt2.SetValue(new Point(px, py), px);
            }
            g.DrawLines(new Pen(Color.White, 2.0f), pt1);
            g.DrawLines(new Pen(Color.Red, 2.0f), pt2);
			
			x += 2;
        }

        private void OnTimer(object sender, EventArgs e)
        {
            // Draw randomly positioned ellipses to the buffer.
            DrawToBuffer(grafx.Graphics);

            // If in bufferingMode 2, draw to the form's HDC.
            if (bufferingMode == 2)
                // Render the graphics buffer to the form's HDC.
                grafx.Render(Graphics.FromHwnd(this.Handle));
            // If in bufferingMode 0 or 1, draw in the paint method.
            else
                this.Refresh();
        }

		private void DrawToBuffer2(Graphics g)
		{
            g.FillRectangle(Brushes.Black, 0, 0, this.Width, this.Height);
			g.DrawBezier(new Pen(Color.Red, 2), pt[0], pt[2], pt[1], pt[3]);

			foreach (Point _pt in pt)
				g.DrawRectangle(new Pen(Color.White, 2), _pt.X, _pt.Y, 1, 1);

			pt[1].X = 300 + (int)(240.0 * Math.Cos(w));
			pt[1].Y = 240 + (int)(240.0 * Math.Sin(w));
			w += 0.01;
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