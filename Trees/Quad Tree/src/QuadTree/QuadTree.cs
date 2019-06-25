using System;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace QuadTree
{
	public class QuadTree<T>
	{
		private Node<T> Root { set; get; }

		public QuadTree() => Root = null;
		public QuadTree(Vector3 max, Vector3 min, Vector3 key, T value) => Root = new Node<T>(max, min, key, value);

		public QuadTree<T> Insert(Vector3 key, T value)
		{
			if (Root == null)
			{
				Root = new Node<T>(key, value);
				return this;
			}

			var node = Root;

			if (node.FindNode(key).Current == default(Node<T>)) // key not found
			{
				Root.Insert(key, value);
			}
			/*while (!node.children.All(x => x == default(Node<T>)))
			{
				if ( key.X < )
			}

			else if (Root.children.All(x => x == default(Node<T>))) //it is leaf
			{
				var currentNode = Root;
				
				
			}*/
			return this;
		}

		

		private Vector3 PointBelongsToSquare(Vector3 point, Node<T> node)
		{

			return Vector3.One;
		}

		public override string ToString()
		{
			Stack<(Node<T>, int)> parents = new Stack<(Node<T>, int)>();
			Node<T> pointer = Root;
			int deepth = 0,
				pointerChildrensIndex = 0;
			StringBuilder result = new StringBuilder();

			while (pointer != default(Node<T>))
			{
				int tab = deepth;

				while (--tab >= 0)
					result.Append(".");

				result.Append(pointer.key + " " + pointer.value.ToString());

				if (pointer.children.Any(x => x != default(Node<T>)) == true)
				{
					for (int i = pointerChildrensIndex; i < pointer.children.Length; ++i)
					{
						if (pointer.children[i] != default(Node<T>))
						{
							parents.Push((pointer, i + 1));
							pointer = pointer.children[i];
							++deepth;
							pointerChildrensIndex = 0;
							break;
						}
					}
					if (deepth == 0)
						break;
				}else
				{
					(pointer, pointerChildrensIndex) = parents.Pop();
					--deepth;
				}

				result.Append("\r\n");
				
			}


			return result.ToString();
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

		public Node<T>[] children;
		public Node<T> parent;
		#endregion

		#region Constructors
		public Node()
		{
			InitNullChildren();
		}

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
			children = InitNullChildren();
			parent = default(Node<T>);
		}

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		private Node<T> InitNewNode(Node<T> parent, Vector3 max, Vector3 min, Vector3 key, T value)
		{

			Node<T> node = new Node<T>()
			{
				children = InitNullChildren(),
				parent = parent,
				max = max,
				min = min,
				key = key,
				value = value
			};

			if (!(node.min.X <= key.X && node.max.X >= key.X &&
				 node.min.Y <= key.Y && node.max.Y >= key.Y &&
				 node.min.Z <= key.Z && node.max.Z >= key.Z))
				throw new Exception("Incorrect key");

			return node;
		}

		private Node<T>[] InitNullChildren() => new Node<T>[8]
		{
			default(Node<T>), default(Node<T>), default(Node<T>), default(Node<T>),
			default(Node<T>), default(Node<T>), default(Node<T>), default(Node<T>)
		};
		
		public Node<T> Insert(Vector3 key, T value)
		{

			var tupleNodes = ChooseBranchNode(key); 

			while (tupleNodes.Current != default(Node<T>))
			{
				tupleNodes = ChooseBranchNode(key);
			}

			if(tupleNodes.Current == default(Node<T>))
			{
				Node<T> node = InitNewNode(tupleNodes.Parent, 
											tupleNodes.Max,
											tupleNodes.Min,
											key,
											value);

				//
				tupleNodes.Parent.children[tupleNodes.Quarter] = node;
				
				return node;
			}

			return default(Node<T>);
		}

		public Node<T> ChooseBranchNode(Node<T> current, Vector3 key)
		{
			if (current.middle.X <= key.X)
			{
				if (current.middle.Y <= key.Y)
				{
					if (current.middle.Z <= key.Z)
						return current.children[0];
					else
						return current.children[1];
				}
				else
				{
					if (current.middle.Z <= key.Z)
						return current.children[2];
					else
						return current.children[3];
				}
			}
			else   // if (current.middle.X > key.X)
			{
				if (current.middle.Y <= key.Y)
				{
					if (current.middle.Z <= key.Z)
						return current.children[4];
					else
						return current.children[5];
				}
				else
				{
					if (current.middle.Z <= key.Z)
						return current.children[6];
					else
						return current.children[7];
				}
			}
		}

		private (Node<T> Current,Node<T> Parent, Vector3 Max, Vector3 Min, int Quarter) ChooseBranchNode(Vector3 key)
		{
			if (middle.X <= key.X)
			{
				if (middle.Y <= key.Y)
				{
					if (middle.Z <= key.Z)
						return (children[0], this, 
							new Vector3(max.X, max.Y, max.Z), 
							new Vector3(middle.X, middle.Y, middle.Z),
							0);
					else
						return (children[1], this, 
							new Vector3(max.X, max.Y, middle.Z),
							new Vector3(middle.X, middle.Y, min.Z),
							1);
				}
				else
				{
					if (middle.Z <= key.Z)
						return (children[2], this,
							new Vector3(max.X, middle.Y, max.Z),
							new Vector3(middle.X, min.Y, middle.Z),
							2);
					else
						return (children[3], this, 
							new Vector3(max.X, middle.Y, middle.Z),
							new Vector3(middle.X, min.Y, min.Z),
							3);
				}
			}
			else   // if (middle.X > key.X)
			{
				if (middle.Y <= key.Y)
				{
					if (middle.Z <= key.Z)
						return (children[4], this,
							new Vector3(middle.X, max.Y, max.Z),
							new Vector3(min.X, middle.Y, middle.Z),
							4);
					else
						return (children[5], this, 
							new Vector3(middle.X, max.Y, middle.Z),
							new Vector3(min.X, middle.Y, min.Z),
							5);
				}
				else
				{
					if (middle.Z <= key.Z)
						return (children[6], this, 
							new Vector3(middle.X, middle.Y, max.Z),
							new Vector3(min.X, min.Y, middle.Z),
							6);
					else
						return (children[7], this,
							new Vector3(middle.X, middle.Y, middle.Z),
							new Vector3(min.X, min.Y, min.Z),
							7);
				}
			}
		}

		public (Node<T> Current, Node<T> Parent) FindNode(Vector3 key)
		{
			var current = this;
			Node<T> parent = default(Node<T>);
			
			while (current != default(Node<T>))
			{
				if (current.key == key)
					return (current, parent);

				parent = current;
				current = ChooseBranchNode(key).Current;
			}

			return (current, parent);
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