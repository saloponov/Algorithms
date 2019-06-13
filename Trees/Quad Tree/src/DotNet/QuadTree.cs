using System.Collections.Generic;
using System.Linq;
using System.Numerics;

namespace QuadTree
{
	public class QuadTree<T>
	{
		private Node<T> Root { set; get; }

		public QuadTree() => Root = new Node<T>();
		public QuadTree(Vector3 max, Vector3 min) => Root = new Node<T>(max, min);

		public void Insert(Vector3 key, T value)
		{
			if (EqualityComparer<T>.Default.Equals(Root.value, default(T)))
			{
				Root.value = value;
				Root.key = key;
			}
			else if (Root.childs.All(x => x == default(Node<T>)))
			{
				var currentNode = Root;
				
				
			}
		}

		private Vector3 PointBelongsToSquare(Vector3 point, Node<T> node)
		{

			return Vector3.One;
		}

		public override string ToString()
		{
			return Root.key + " " + Root.value.ToString();
		}
	}

	internal class Node<T>
	{
		public Node()
		{
			max = new Vector3(float.MaxValue, float.MaxValue, float.MaxValue);
			min = new Vector3(float.MinValue, float.MinValue, float.MinValue);

			childs = new Node<T>[4] { default(Node<T>), default(Node<T>), default(Node<T>), default(Node<T>) };
			parent = default(Node<T>);
			value =  default(T);
		}

		public Node(Vector3 max, Vector3 min)
		{
			this.max = max; this.min = min;
		}

		
		private Vector3 max { get; set; }
		private Vector3 min { get; set; }

		private Vector3 middle => (max + min) / 2;

		public Vector3 key { get; set; }

		public T value 
		{
			get;
			set;
		}

		public Node<T>[] childs;
		public Node<T> parent;
	}
}
