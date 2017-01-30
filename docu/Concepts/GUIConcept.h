

namespace GUI
{

/*
The GUI is not designed with the model-view-controller pattern.
This is due to the too complex class hierarchy without a complex data model,
i.e. a push button for instance can be implemented easily with few data fields,
no complex hierarchy is necessary.
Thus each GUI element is implemented as a single class, such as "Button" without "ButtonModel" and "ButtonView".
On the other hand, an extensive event receiver system is used here.
*/

typedef Math::Size2i Size;
typedef Math::Point2i Point;
typedef Math::Recti Rect;

//! Base GUI element class. Each button, checkbox, listview etc. controls a respective view object.
class ViewController
{
	
	public:
		
		class EventHandler
		{
			
			public:
				
				class Event
				{
					public:
						Event(ViewController* owner) :
							owner_(owner)
						{
						}
						virtual ~Event()
						{
						}
						inline void Veto()
						{
							isVeto_ = true;
						}
						inline bool IsVeto() const
						{
							return isVeto_;
						}
						//! Returns the owner (or rather event originator).
						inline ViewController* GetOwner() const
						{
							return owner_;
						}
					private:
						ViewController*	owner_	= nullptr;
						bool			isVeto_	= false;
				};
				
				class SizeEvent : public Event
				{
					public:
						SizeEvent(ViewController* owner, const Size& size) :
							Event(owner),
							size(size)
						{
						}
						Size size;
				};
				
				class MoveEvent : public Event
				{
					public:
						MoveEvent(ViewController* owner, const Point& position) :
							Event(owner),
							position(position)
						{
						}
						Point position;
				};
				
				class TriggerEvent : public Event
				{
					public:
						TriggerEvent(ViewController* owner, unsigned int id) :
							Event(owner),
							id_(id)
						{
						}
						inline unsigned int GetId() const
						{
							return id_;
						}
					private:
						unsigned int id_;
				};
				
				class MouseEvent : public Event
				{
					public:
						MouseEvent(ViewController* owner) :
							Event(owner)
						{
						}
					private:
						IO::MouseKeyCodes keyCode_;
						Point mousePos_;
				};
				
				class DrawEvent : public Event
				{
					public:
						DrawEvent(ViewController* owner) :
							Event(owner)
						{
						}
					private:
						Video::RenderContext* renderContext_;
						Rect parentClientArea_;
				};
				
				typedef Event FocusEvent;
				
				virtual void OnGetFocus(FocusEvent& event)
				{
				}
				virtual void OnLostFocus(FocusEvent& event)
				{
				}
				
				virtual void OnResize(SizeEvent& event)
				{
				}
				virtual void OnMove(MoveEvent& event)
				{
				}
				
				virtual void OnMouseDown(MouseEvent& event)
				{
				}
				virtual void OnMouseUp(MouseEvent& event)
				{
				}
				virtual void OnMouseMove(MouseEvent& event)
				{
				}
				
				virtual void OnDraw(DrawEvent& event)
				{
				}
				
				//! Triggers occur when a frame is maximized, minimized or closed.
				virtual void OnTrigger(TriggerEvent& event)
				{
				}
				
		};
		
		inline Rect Area() const
		{
			return { position_, size_ };
		}
		
		virtual void Update() = 0;
		
		virtual void Draw() = 0;
		
		Point GlobalPosition() const;
		
		//! Returns the client area relative to the view controller's position.
		virtual Rect ClientArea() const;
		
		//! Sets the new position relative to the view controller's parent position.
		virtual void Move(const Point& position);
		inline const Point& GetPosition() const
		{
			return position_;
		}
		
		virtual void Resize(const Size& size);
		inline const Size& GetSize() const
		{
			return size_;
		}
		
		virtual void ChangeTitle(const std::wstring& title);
		inline const std::wstring& GetTitle() const
		{
			return title_;
		}
		
		virtual void ChangeParent(ViewController* parent);
		inline ViewController* GetParent() const
		{
			return parent_;
		}
		
		virtual void Show(bool show = true);
		inline bool IsVisible() const
		{
			return isVisible_;
		}
		
		virtual void Enable(bool enabled);
		inline bool IsEnabled() const
		{
			return isEnabled_;
		}
		
	private:
		
		ViewController* parent_ = nullptr;
		std::vector<ViewControllerPtr> children_;
		
		Point position_;
		Size size_;
		
		std::wstring title_;
		
		bool isVisible_ = true;
		bool isEnabled_ = true;
		
};

class Frame : public ViewController
{
	
	public:
		
		
};

class Button : public ViewController
{
	
	public:
		
		
		
	private:
		
		
		
};


} // /namespace GUI

