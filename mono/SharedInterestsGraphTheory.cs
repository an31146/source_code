/*
 * SharedInterestsGraphTheory.cs
 *
 * "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\Roslyn\csc.exe" /target:exe /out:SharedInterestsGraphTheory.exe SharedInterestsGraphTheory.cs
 */
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text.RegularExpressions;
using System.Text;
using System;



class Result
{

    /*
     * Complete the 'maxShared' function below.
     *
     * The function is expected to return an INTEGER.
     * The function accepts WEIGHTED_INTEGER_GRAPH friends as parameter.
     */

    /*
     * For the weighted graph, <name>:
     *
     * 1. The number of nodes is <name>Nodes.
     * 2. The number of edges is <name>Edges.
     * 3. An edge exists between <name>From[i] and <name>To[i]. The weight of the edge is <name>Weight[i].
     *
     */

    public static int maxShared(int friendsNodes, List<int> friendsFrom, List<int> friendsTo, List<int> friendsWeight)
    {
        Dictionary<string, int> ProductOfEachEdgeWeights = new Dictionary<string, int>();
        //Dictionary<string, int> EdgeWeights = new Dictionary<string, int>();
        
        for (int i = 0; i < friendsNodes; i++)
        {
            string strEdge = friendsFrom[i].ToString() + friendsTo[i].ToString();
            
            if (!ProductOfEachEdgeWeights.ContainsKey(strEdge))
            {
                ProductOfEachEdgeWeights.Add(strEdge, friendsWeight[i]);
            }
            else
            {
                ProductOfEachEdgeWeights[strEdge] *= friendsWeight[i];
            }
        }
        
        int maxWeight = 0;
        foreach (int product in ProductOfEachEdgeWeights.Values)
        {
            if (product > maxWeight)
                maxWeight = product;
        }
        
        return maxWeight;
    }

}

class Solution
{
    public static void Main(string[] args)
    {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        string[] friendsNodesEdges = Console.ReadLine().TrimEnd().Split(' ');

        int friendsNodes = Convert.ToInt32(friendsNodesEdges[0]);
        int friendsEdges = Convert.ToInt32(friendsNodesEdges[1]);

        List<int> friendsFrom = new List<int>();
        List<int> friendsTo = new List<int>();
        List<int> friendsWeight = new List<int>();

        for (int i = 0; i < friendsEdges; i++)
        {
            string[] friendsFromToWeight = Console.ReadLine().TrimEnd().Split(' ');

            friendsFrom.Add(Convert.ToInt32(friendsFromToWeight[0]));
            friendsTo.Add(Convert.ToInt32(friendsFromToWeight[1]));
            friendsWeight.Add(Convert.ToInt32(friendsFromToWeight[2]));
        }

        int result = Result.maxShared(friendsNodes, friendsFrom, friendsTo, friendsWeight);

        textWriter.WriteLine(result);

        textWriter.Flush();
        textWriter.Close();
    }
}
