// Redirection Utility
// Author: Hai Vu (haivu2004 on Google mail)

using System;
using System.IO;

namespace RedirectIO
{
    /// <summary>
    /// OutToFile is an easy way to redirect console output to a file.
    ///
    /// Usage:
    ///    Console.WriteLine("This text goes to the console by default");
    ///    using (OutToFile redir = new OutToFile("out.txt"))
    ///    {
    ///        Console.WriteLine("Contents of out.txt");
    ///    }
    ///    Console.WriteLine("This text goes to console again");
    ///
    /// </summary>
    public class OutToFile : IDisposable
    {
        private StreamWriter fileOutput;
        private TextWriter oldOutput;

        /// <summary>
        /// Create a new object to redirect the output
        /// </summary>
        /// <param name="outFileName">
        /// The name of the file to capture console output
        /// </param>
        public OutToFile(string outFileName)
        {
            oldOutput = Console.Out;
            fileOutput = new StreamWriter(
                new FileStream(outFileName, FileMode.Create)
                );
            fileOutput.AutoFlush = true;
            Console.SetOut(fileOutput);
        }

        // Dispose() is called automatically when the object
        // goes out of scope
        public void Dispose()
        {
            Console.SetOut(oldOutput);  // Restore the console output
            fileOutput.Close();        // Done with the file
        }
    }
}