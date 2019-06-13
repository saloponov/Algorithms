using System;
using System.Collections.Generic;
using System.IO.Enumeration;
using System.Linq;
using System.Numerics;
using System.Threading;
using System.Runtime.CompilerServices;
using System.Security.Cryptography.X509Certificates;
using System.Transactions;


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
			}

			var node = Root;
			/*while (!node.childs.All(x => x == default(Node<T>)))
			{
				if ( key.X < )
			}

			else if (Root.childs.All(x => x == default(Node<T>))) //it is leaf
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
		#region Fields
		private Vector3 max { get; set; }
		private Vector3 min { get; set; }

		public Vector3 middle => (max + min) / 2;

		public Vector3 key { get; set; }

		public T value
		{
			get;
			set;
		}

		public Node<T>[] childs;
		public Node<T> parent;
		#endregion

		#region Constructors
		/*public Node()
		{
			max = new Vector3(float.MaxValue, float.MaxValue, float.MaxValue);
			min = new Vector3(float.MinValue, float.MinValue, float.MinValue);

			InitRootNode();
			value = default(T);
		}*/

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
		#endregion

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
		private Node<T>[] InitNullChilds() => childs = new Node<T>[8]
		{
			default(Node<T>), default(Node<T>), default(Node<T>), default(Node<T>),
			default(Node<T>), default(Node<T>), default(Node<T>), default(Node<T>)
		};
		
		public static Node<T> ChooseBranchNode(Node<T> current, Vector3 key)
		{
			if (current.middle.X <= key.X)
			{
				if (current.middle.Y <= key.Y)
				{
					if (current.middle.Z <= key.Z)
						return current.childs[0];
					else
						return current.childs[1];
				}
				else
				{
					if (current.middle.Z <= key.Z)
						return current.childs[2];
					else
						return current.childs[3];
				}
			}
			else   // if (current.middle.X > key.X)
			{
				if (current.middle.Y <= key.Y)
				{
					if (current.middle.Z <= key.Z)
						return current.childs[4];
					else
						return current.childs[5];
				}
				else
				{
					if (current.middle.Z <= key.Z)
						return current.childs[6];
					else
						return current.childs[7];
				}
			}
		}

		private Node<T> ChooseBranchNode(Vector3 key)
		{
			if (middle.X <= key.X)
			{
				if (middle.Y <= key.Y)
				{
					if (middle.Z <= key.Z)
						return childs[0];
					else
						return childs[1];
				}
				else
				{
					if (middle.Z <= key.Z)
						return childs[2];
					else
						return childs[3];
				}
			}
			else   // if (middle.X > key.X)
			{
				if (middle.Y <= key.Y)
				{
					if (middle.Z <= key.Z)
						return childs[4];
					else
						return childs[5];
				}
				else
				{
					if (middle.Z <= key.Z)
						return childs[6];
					else
						return childs[7];
				}
			}
		}

		public (Node<T>, Node<T>) FindNode(Vector3 key)
		{
			var current = this;
			Node<T> parent = default(Node<T>);
			
			while (current != default(Node<T>))
			{
				if (current.key == key)
					return (parent, current);

				parent = current;
				current = ChooseBranchNode(key);
			}

			return (parent, current);
		}

		public static Node<T> FindNode(Node<T> current, Vector3 key)
		{
			Node<T> parent = current;
			while (current != default(Node<T>))
			{
				if (current.key == key)
					return current;

				current.ChooseBranchNode(key);
			}
			return current;
		}
	}
}
