using System;
using System.Windows.Forms;

namespace myNamespace
{
    class Program
    {
        static void Main()
        {
            const string message = "Are you sure that you would like to close the form?";
            const string caption = "Form Closing";
            
            var result = MessageBox.Show(message, caption,
                                         MessageBoxButtons.YesNo,
                                         MessageBoxIcon.Question);

            // If the No button was pressed ...
            if (result == DialogResult.No)
            {
                // cancel the closure of the form.
                Console.WriteLine("You selected: No");
            }
            else
            {
                Console.WriteLine("You selected: Yes");
            }
        }
    }
}
