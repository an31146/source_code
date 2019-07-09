
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

public class Challenge {
    public static long Solution(long[] prices) {
        // Type your solution here
        //long maxPrice = prices.Max, minPrice = prices.Min;

        // Type your solution here
        List<long> listOfPrices = new List<long>(prices);
        long lastPrice = listOfPrices[0], buyPrice = 0, sellPrice = 0;
        List<long> dailyPrices = new List<long>();
        long totalProfit = 0;
        bool bBought = false;

        foreach (long l in listOfPrices)
        {
            dailyPrices.Add(l);
            Console.WriteLine("{0}", l);
            if (l < dailyPrices.Min() && !bBought)
            {
                buyPrice = l;
                bBought = true;
                Console.WriteLine("buyPrice: {0}", buyPrice);
            }
            else if (bBought && l > buyPrice)
            {
                sellPrice = l;
                totalProfit += sellPrice - buyPrice;
                bBought = false;
                dailyPrices.Clear();
                
                Console.WriteLine("\n{0} - ({1})\ntotalProfit: {2}", sellPrice, buyPrice, totalProfit);
            }
            //lastPrice = l;
        }
        
        return totalProfit;
    }

    public static void Main()
    {
        long[] arr = {13,10,8,4,10};

        Console.WriteLine(Solution(arr));
    }
}

