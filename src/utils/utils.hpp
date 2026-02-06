namespace ShiraNet::Utils {
    
    class NoCopy {
	public:
		NoCopy(const NoCopy&) = delete;
		NoCopy& operator=(const NoCopy&) = delete;
		NoCopy() = default;
	};

}