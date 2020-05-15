#define _TEXTFILE

using System;
using System.IO;
using System.Reflection;
using System.Text;
using System.Collections.Generic;
using System.Diagnostics;

public class Tree<T> : List<T>
{
    //private Tree<T> Node;
    private T Value;
    private Comparer<T> defComp;
    private int UniqueCount;
    private int TotalCount = 0;
    private Tree<T> Left;
    private Tree<T> Right;
    
    public Tree(T _value)
    {
        defComp = Comparer<T>.Default;
        UniqueCount = 1;
        Value = _value;
        Left = null;
        Right = null;
        // Console.WriteLine("created node: {0}\n", Value);
    }
       
    public Tree<T> GetNode() => this;
    
    public Tree<T> GetLeft() => Left;
    
    public Tree<T> GetRight() => Right;
    
    public int GetUniqueCount() => UniqueCount;

    public int GetTotalCount() => TotalCount;
    
    public T GetValue() => (T)Value;

    public int CompareTo(T y) => defComp.Compare(Value, y);

    public void AddNode(Tree<T> _thisNode, T _value)
    {
        if (_thisNode == null)
        {   
            //Node = new Tree<T>(_value);
            _thisNode = new Tree<T>(_value);
            //Node = _thisNode;
            //if (_thisNode == null)
            //    _thisNode = Node;
            //Console.WriteLine("added node: {0}", _thisNode.GetValue());
            return;
        }
            
        // Console.WriteLine("_thisNode.Value = {0}", this.Value);
        
        if (_thisNode.CompareTo(_value) == 0)
        {
            _thisNode.UniqueCount++;
            Console.WriteLine("incremented unique count of recurring words to: {0}", _thisNode.UniqueCount);
            
        } else if (_thisNode.CompareTo(_value) > 0)
            {
                if (_thisNode.Left == null)
                {
                    _thisNode.Left = new Tree<T>(_value);
                    Console.WriteLine("added \"{0}\" to Left node.", _value);
                    // Console.WriteLine("_thisNode.Left == null is {0}\n_thisNode.Right == null is {1}\n", _thisNode.Left == null, _thisNode.Right == null);
                    Console.WriteLine("_thisNode.Left {0}= null\n_thisNode.Right {1}= null\n", (_thisNode.Left == null ? '=' : '!'), (_thisNode.Right == null ? '=' : '!') );
                    Console.WriteLine(new String('-', 40));
                }
                else
                    AddNode(_thisNode.Left, _value);
                
                //    return this.Left;
                //else
                //    _thisNode.Left = new Tree<T>(_value);
                
            } else {
                if (_thisNode.Right == null)
                {
                    _thisNode.Right = new Tree<T>(_value);
                    Console.WriteLine("added \"{0}\" to Right node.", _value);
                    // Console.WriteLine("_thisNode.Left == null is {0}\n_thisNode.Right == null is {1}\n", _thisNode.Left == null, _thisNode.Right == null);
                    Console.WriteLine("_thisNode.Left {0}= null\n_thisNode.Right {1}= null\n", (_thisNode.Left == null ? '=' : '!'), (_thisNode.Right == null ? '=' : '!') );
                    Console.WriteLine(new String('-', 40));
                } 
                else
                    AddNode(_thisNode.Right, _value);
                
                //    return this.Right;
                //else
                //    _thisNode.Right = new Tree<T>(_value);
                
            }
        
        // Console.Write("This statement should never be reached...");
        // Console.ReadLine();
    }
    
    public List<T> Traverse(Tree<T> _node, List<T> _listOfT = null)
    {
        if (_listOfT == null)
            _listOfT = new List<T>();
            
        //Console.WriteLine("_listOfT.Count = {0}", _listOfT.Count);
        
        //if (this.CompareTo(_node.GetValue()) <= 0)
        if (_node != null)
        {
            //_listOfT.Add(_node.Left.GetValue());
            Traverse(_node.Left, _listOfT);
            _listOfT.Add(_node.GetValue());
            Console.WriteLine("{0,20}\tcount: {1}", String.Format("\"{0}\"", _node.GetValue()), _node.UniqueCount);
            TotalCount += _node.UniqueCount;
            Traverse(_node.Right, _listOfT);
        }
        
        return _listOfT;
    }
} // Tree

public class Example
{
    public static void Main(string[] args)
    {
        // Create the link list.
        string[] words =
        //    { "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" };
            { "mangetout", "avocado", "cabbage", "beetroot", "capsicum", "fennel", "carrot", "ginger", "garlic", "kale", "potato", "avocado", "carrot", "coriander", "carrot" };
        LinkedList<string> sentence = new LinkedList<string>(words);
        
            Tree<string> tree = new Tree<string>("root");

#if _TEXTFILE
        // Read a text file and parse each word into a list!
        int lineCount = 0;
        
        // StreamReader sr = new StreamReader(@"C:\Users\richa\Google Drive\Documents\Text Files\IRC01.txt");
        string fileName = @"C:\Users\richa\Google Drive\Documents\Text Files\iManO2k.AutoMacro module.txt";
        
        // Why do things the easy way?  ;-)
        string [] args2 = System.Diagnostics.Process.GetCurrentProcess().StartInfo.Arguments.Split('"');
        if (args.Length == 1)
            fileName = args[0];
        
        // Console.WriteLine(fileName);
        // Console.ReadLine();
        
        StreamReader sr = null;;
        try
        {
            sr = new StreamReader(fileName);

            do {
                string strLineIn = sr.ReadLine();
                Console.WriteLine("Line {0,5} - Length: {1}", ++lineCount, strLineIn.Length);
            
                words = strLineIn.Split(new char [] {' ',',','.','?',';',':','!','"','\'','-','_','(',')','*','/'} );
#endif
                foreach (string str in words)
                {
                    //if (words[0].Equals(str))
                    //    continue;
                    //else 
                    string trimmed = str.Trim().ToLower(); 
                    //Console.WriteLine(str);
                    if (!string.IsNullOrEmpty(trimmed))
                        tree.AddNode(tree, trimmed);
                    
                    var t = tree;
                    // Console.WriteLine("{0}\t{1}\t{2}", t.GetValue(), t.GetLeft(), t.GetRight());
                }
#if _TEXTFILE
            } while (!sr.EndOfStream);
        }
        catch (IOException ex)
        {
            Console.WriteLine("\nCaught exception: {0}", ex.Message);
            return;
        }
        finally
        {
            if (sr != null)
                sr.Close();
        }
#endif        
        // Console.WriteLine("\ntree != null is {0}\ntree.GetLeft() != null is {1}\ntree.GetRight() != null is {2}\n", tree != null, tree.GetLeft() != null, tree.GetRight() != null);
        Console.WriteLine("\ntree {0}= null\ntree.GetLeft() {1}= null\ntree.GetRight() {2}= null\n", 
                            (tree == null ? '=' : '!'), (tree.GetLeft() == null ? '=' : '!'), (tree.GetRight() == null ? '=' : '!') );
        
        List<string> sorted_words = null;
        
        sorted_words = tree.Traverse(tree, sorted_words);
        Console.WriteLine("\nsorted_words.Count = {0}\ntree.GetTotalCount = {1}\n", sorted_words.Count, tree.GetTotalCount());
        
        // foreach (string str in sorted_words)
            // Console.WriteLine(str);

        Console.Write("Press Enter: ");
        Console.ReadLine();
    }   // void Main
}   // class