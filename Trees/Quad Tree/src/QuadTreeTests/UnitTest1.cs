using System;
using Xunit;
using QuadTree;
using System.Numerics;

namespace QuadTreeTests
{
    public class UnitTest1
    {
        [Fact]
        public void CreateOnlyRootNode()
        {
			Assert.Equal("<1  1  1> 124", new QuadTree<string>().Insert(Vector3.One, "124").ToString());
		}

        string FindNode()
        {
	        return "";
        }
    }
}
