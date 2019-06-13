using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Threading;
using System.Runtime.CompilerServices;


namespace QuadTree
{
	public class QuadTree<T>
	{
		private Node<T> Root { set; get; }

		public QuadTree() => Root = null;
		public QuadTree(Vector3 max, Vector3 min, Vector3 key, T value) => Root = new Node<T>(max, min, key, value);

		public void Insert(Vector3 key, T value)
		{
			//if (EqualityComparer<T>.Default.Equals(Root.value, default(T)))
			if (Root == null)
			{
				Root = new Node<T>(key, value);
			}/*
			else if (Root.childs.All(x => x == default(Node<T>)))
			{
				var currentNode = Root;
				
				
			}*/
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

			InitRootNode();
			value = default(T);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		private void InitRootNode()
		{
			InitNullChilds();
			parent = default(Node<T>);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		private void InitNewNode(Node<T> parent)
		{
			InitNullChilds();
			this.parent = parent;
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		private Node<T>[] InitNullChilds() => childs = new Node<T>[4] { default(Node<T>), default(Node<T>), default(Node<T>), default(Node<T>) };
		
		public Node(Vector3 max, Vector3 min)
		{
			this.max = max; this.min = min;

			InitRootNode();
		}

		public Node(Vector3 max, Vector3 min, Vector3 key, T value)
		{
			this.max = max; this.min = min;
			this.key = key;
			this.value = value;

			InitRootNode();
		}

		public Node(Vector3 key, T value)
		{
			this.max = key + Vector3.One * 100;
			this.min = key - Vector3.One * 100;
			this.key = key;
			this.value = value;

			InitRootNode();
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
