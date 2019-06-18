using System;
using System.Numerics;
using QuadTree;

namespace DotNet
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            QuadTree<int> tree = new QuadTree<int>();
			tree.Insert(Vector3.One, 202);
			tree.Insert(Vector3.One * 2, 205);
			Console.WriteLine(tree.ToString());

		}
    }
}