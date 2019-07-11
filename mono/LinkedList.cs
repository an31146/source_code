using System;
using System.Reflection;
using System.Text;
using System.Collections.Generic;

public class Tree<T> : List<T>
{
    //private Tree<T> Node;
    private T Value;
    private Comparer<T> defComp;
    private Tree<T> Left;
    private Tree<T> Right;
    
    public Tree(T _value)
    {
        defComp = Comparer<T>.Default;
        Value = _value;
        Left = null;
        Right = null;
    }
       
    public Tree<T> GetNode()
    {
        return this;
    }
    
    public Tree<T> GetLeft()
    {
        return Left;
    }
    
    public Tree<T> GetRight()
    {
        return Right;
    }
    
    public T GetValue()
    {
        return (T)Value;
    }

    public int CompareTo(T y)
    {
        return defComp.Compare(Value, y);
    }

    public Tree<T> AddNode(ref Tree<T> _thisNode, T _value)
    {
        if (_thisNode == null)
        {
            //Node = new Tree<T>(_value);
            _thisNode = new Tree<T>(_value);
            //Node = _thisNode;
            //if (_thisNode == null)
            //    _thisNode = Node;
            Console.WriteLine("added node: {0}", _thisNode.GetValue());
            return _thisNode;
        }
            
        if (this.CompareTo(_value) > 0)
        {
            Console.WriteLine("add \"{0}\" to left node.", _value);
            //if (_thisNode.Left == null)
                this.Left = AddNode(ref _thisNode.Left, _value);
                Console.WriteLine("_thisNode.Left == null is {0}\n_thisNode.Right == null is {1}\n", _thisNode.Left == null, _thisNode.Right == null);
                return this.Left;
            //else
            //    _thisNode.Left = new Tree<T>(_value);
            
        }
        else
        {
            Console.WriteLine("add \"{0}\" to right node.", _value);
            //if (_thisNode.Right != null)
                this.Right = AddNode(ref _thisNode.Right, _value);
                Console.WriteLine("_thisNode.Left == null is {0}\n_thisNode.Right == null is {1}\n", _thisNode.Left == null, _thisNode.Right == null);
                return this.Right;
            //else
            //    _thisNode.Right = new Tree<T>(_value);
            
        }
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
            Console.WriteLine(_node.GetValue());
            Traverse(_node.Right, _listOfT);
        }
        
        return _listOfT;
    }
} // Tree

public class Example
{
    public static void Main()
    {
        // Create the link list.
        string[] words =
            { "the", "fox", "jumps", "over", "the", "dog" };
        LinkedList<string> sentence = new LinkedList<string>(words);
        
        
        Tree<string> tree = new Tree<string>("root");
        foreach (string str in words)
        {
            //if (words[0].Equals(str))
            //    continue;
            //else 
             
            //Console.WriteLine(str);
            tree.AddNode(ref tree, str);
        }
        
        Console.WriteLine("\ntree != null is {0}\ntree.GetLeft() != null is {1}\ntree.GetRight() != null is {2}\n", tree != null, tree.GetLeft() != null, tree.GetRight() != null);
        
        List<string> sorted_words = null;
        
        sorted_words = tree.Traverse(tree, sorted_words);
        Console.WriteLine("sorted_words.Count = {0}", sorted_words.Count);
        
        foreach (string str in sorted_words)
            Console.WriteLine(str);

        Console.Write("Press Enter: ");
        Console.ReadLine();

        return;
        
        Display(sentence, "The linked list values:");
        Console.WriteLine("sentence.Contains(\"jumps\") = {0}",
            sentence.Contains("jumps"));

        // Add the word 'today' to the beginning of the linked list.
        sentence.AddFirst("today");
        Display(sentence, "Test 1: Add 'today' to beginning of the list:");

        // Move the first node to be the last node.
        LinkedListNode<string> mark1 = sentence.First;
        sentence.RemoveFirst();
        sentence.AddLast(mark1);
        Display(sentence, "Test 2: Move first node to be last node:");

        // Change the last node to 'yesterday'.
        sentence.RemoveLast();
        sentence.AddLast("yesterday");
        Display(sentence, "Test 3: Change the last node to 'yesterday':");

        // Move the last node to be the first node.
        mark1 = sentence.Last;
        sentence.RemoveLast();
        sentence.AddFirst(mark1);
        Display(sentence, "Test 4: Move last node to be first node:");


        // Indicate the last occurence of 'the'.
        sentence.RemoveFirst();
        LinkedListNode<string> current = sentence.FindLast("the");
        IndicateNode(current, "Test 5: Indicate last occurence of 'the':");

        // Add 'lazy' and 'old' after 'the' (the LinkedListNode named current).
        sentence.AddAfter(current, "old");
        sentence.AddAfter(current, "lazy");
        IndicateNode(current, "Test 6: Add 'lazy' and 'old' after 'the':");

        // Indicate 'fox' node.
        current = sentence.Find("fox");
        IndicateNode(current, "Test 7: Indicate the 'fox' node:");

        // Add 'quick' and 'brown' before 'fox':
        sentence.AddBefore(current, "quick");
        sentence.AddBefore(current, "brown");
        IndicateNode(current, "Test 8: Add 'quick' and 'brown' before 'fox':");

        // Keep a reference to the current node, 'fox',
        // and to the previous node in the list. Indicate the 'dog' node.
        mark1 = current;
        LinkedListNode<string> mark2 = current.Previous;
        current = sentence.Find("dog");
        IndicateNode(current, "Test 9: Indicate the 'dog' node:");

        // The AddBefore method throws an InvalidOperationException
        // if you try to add a node that already belongs to a list.
        Console.WriteLine("Test 10: Throw exception by adding node (fox) already in the list:");
        try
        {
            sentence.AddBefore(current, mark1);
        }
        catch (InvalidOperationException ex)
        {
            Console.WriteLine("Exception message: {0}", ex.Message);
        }
        Console.WriteLine();

        // Remove the node referred to by mark1, and then add it
        // before the node referred to by current.
        // Indicate the node referred to by current.
        sentence.Remove(mark1);
        sentence.AddBefore(current, mark1);
        IndicateNode(current, "Test 11: Move a referenced node (fox) before the current node (dog):");

        // Remove the node referred to by current.
        sentence.Remove(current);
        IndicateNode(current, "Test 12: Remove current node (dog) and attempt to indicate it:");

        // Add the node after the node referred to by mark2.
        sentence.AddAfter(mark2, current);
        IndicateNode(current, "Test 13: Add node removed in test 11 after a referenced node (brown):");

        // The Remove method finds and removes the
        // first node that that has the specified value.
        sentence.Remove("old");
        Display(sentence, "Test 14: Remove node that has the value 'old':");

        // When the linked list is cast to ICollection(Of String),
        // the Add method adds a node to the end of the list.
        sentence.RemoveLast();
        ICollection<string> icoll = sentence;
        icoll.Add("rhinoceros");
        Display(sentence, "Test 15: Remove last node, cast to ICollection, and add 'rhinoceros':");

        Console.WriteLine("Test 16: Copy the list to an array:");
        // Create an array with the same number of
        // elements as the inked list.
        string[] sArray = new string[sentence.Count];
        sentence.CopyTo(sArray, 0);

        foreach (string s in sArray)
        {
            Console.WriteLine(s);
        }

        // Release all the nodes.
        sentence.Clear();

        Console.WriteLine();
        Console.WriteLine("Test 17: Clear linked list. Contains 'jumps' = {0}",
            sentence.Contains("jumps"));

        Console.Write("Press Enter: ");
        Console.ReadLine();
    }

    private static void Display(LinkedList<string> words, string test)
    {
        Console.WriteLine(test);
        foreach (string word in words)
        {
            Console.Write(word + " ");
        }
        Console.WriteLine();
        Console.WriteLine();
    }

    private static void IndicateNode(LinkedListNode<string> node, string test)
    {
        Console.WriteLine(test);
        if (node.List == null)
        {
            Console.WriteLine("Node '{0}' is not in the list.\n",
                node.Value);
            return;
        }

        StringBuilder result = new StringBuilder("(" + node.Value + ")");
        LinkedListNode<string> nodeP = node.Previous;

        while (nodeP != null)
        {
            result.Insert(0, nodeP.Value + " ");
            nodeP = nodeP.Previous;
        }

        node = node.Next;
        while (node != null)
        {
            result.Append(" " + node.Value);
            node = node.Next;
        }

        Console.WriteLine(result);
        Console.WriteLine();
    }
}

//This code example produces the following output:
//
//The linked list values:
//the fox jumps over the dog

//Test 1: Add 'today' to beginning of the list:
//today the fox jumps over the dog

//Test 2: Move first node to be last node:
//the fox jumps over the dog today

//Test 3: Change the last node to 'yesterday':
//the fox jumps over the dog yesterday

//Test 4: Move last node to be first node:
//yesterday the fox jumps over the dog

//Test 5: Indicate last occurence of 'the':
//the fox jumps over (the) dog

//Test 6: Add 'lazy' and 'old' after 'the':
//the fox jumps over (the) lazy old dog

//Test 7: Indicate the 'fox' node:
//the (fox) jumps over the lazy old dog

//Test 8: Add 'quick' and 'brown' before 'fox':
//the quick brown (fox) jumps over the lazy old dog

//Test 9: Indicate the 'dog' node:
//the quick brown fox jumps over the lazy old (dog)

//Test 10: Throw exception by adding node (fox) already in the list:
//Exception message: The LinkedList node belongs a LinkedList.

//Test 11: Move a referenced node (fox) before the current node (dog):
//the quick brown jumps over the lazy old fox (dog)

//Test 12: Remove current node (dog) and attempt to indicate it:
//Node 'dog' is not in the list.

//Test 13: Add node removed in test 11 after a referenced node (brown):
//the quick brown (dog) jumps over the lazy old fox

//Test 14: Remove node that has the value 'old':
//the quick brown dog jumps over the lazy fox

//Test 15: Remove last node, cast to ICollection, and add 'rhinoceros':
//the quick brown dog jumps over the lazy rhinoceros

//Test 16: Copy the list to an array:
//the
//quick
//brown
//dog
//jumps
//over
//the
//lazy
//rhinoceros

//Test 17: Clear linked list. Contains 'jumps' = False
//
