#ifndef __interpolator_h_
#define __interpolator_h_



template <class V, class T>
class Interpolator {

	private:

	protected:

		V * m_v;
		T * m_t;
		int m_size;
		bool m_ownData;

	public:

		Interpolator(void) {
			this->m_v = 0;
			this->m_t = 0;
			this->m_size = 0;
			this->m_ownData = false;
		}

		Interpolator(V * vs, T * ts, int size, bool doCopy = true) {
			this->m_v = 0;
			this->m_t = 0;
			this->m_size = 0;
			this->m_ownData = false;

			this->Set(vs, ts, size, doCopy);
		}

		virtual ~Interpolator(void) {
			this->Clear();
		}

		int GetKeyFrame(const T & t) const {
			int i = 0;
			while (t >= this->m_t[i + 1]) {
				i++;
			}

			return (i);
		}

		bool OwnData(void) const {
			return (this->m_ownData);
		}

		bool Set(V * vs, T * ts, int size, bool doCopy = true) {
			this->Clear();

			if (doCopy) {
				int i;

				this->m_v = new V[size];
				if (this->m_v == 0) {
					return (false);
				}
				this->m_t = new T[size];
				if (this->m_t == 0) {
					delete [] this->m_v;
					this->m_v = 0;
					return (false);
				}

				for (i=0; i<size; ++i) {
					this->m_v[i] = vs[i];
					this->m_t[i] = ts[i];
				}
			}
			else {
				this->m_v = vs;
				this->m_t = ts;
			}

			this->m_size = size;
			this->m_ownData = doCopy;

			return (true);
		}

		void Clear(void) {
			if (this->m_ownData) {
				delete [] this->m_v;
				delete [] this->m_t;
			}

			this->m_v = 0;
			this->m_t = 0;
			this->m_size = 0;
			this->m_ownData = false;
		}

		int Size(void) const {
			return (this->m_size);
		}

		V & Value(int i) {
			return (this->m_v[i]);
		}

		const V & Value(int i) const {
			return (this->m_v[i]);
		}

		T & Time(int i) {
			return (this->m_t[i]);
		}

		const T & Time(int i) const {
			return (this->m_t[i]);
		}

		virtual V Interpolate(const T & t) const = 0;

};

template <class V, class T>
class LinearInterpolator : public Interpolator<V, T> {

	public:

		LinearInterpolator(void) : Interpolator<V, T>() {
		}

		LinearInterpolator(V * vs, T * ts, int size, bool doCopy = true) : Interpolator<V, T>(vs, ts, size, doCopy) {
		}

		virtual V Interpolate(const T & t) const {
			if (this->m_size <= 0) {
				return (V());
			}

			if (this->m_size == 1) {
				return (this->m_v[0]);
			}

			if (t <= this->m_t[0]) {
				return (this->m_v[0]);
			}

			if (t >= this->m_t[this->m_size - 1]) {
				return (this->m_v[this->m_size - 1]);
			}

			int i = this->GetKeyFrame(t);

			const T dt = this->m_t[i + 1] - this->m_t[i];

			if (dt <= T(0)) {
				return (this->m_v[i]);
			}

			const T d = (t - this->m_t[i]) / dt;

			const V v0 = this->m_v[i];
			const V v1 = this->m_v[i + 1];

			const V r = v0 + ((v1 - v0) * d);

			return (r);
		}

};

template <class V, class T>
class CatmullRomInterpolator : public Interpolator<V, T> {

	public:

		CatmullRomInterpolator(void) : Interpolator<V, T>() {
		}

		CatmullRomInterpolator(V * vs, T * ts, int size, bool doCopy = true) : Interpolator<V, T>(vs, ts, size, doCopy) {
		}

		virtual V Interpolate(const T & t) const {
			if (this->m_size <= 0) {
				//return (V(0.0));
				return (V());
			}

			if (t <= this->m_t[0]) {
				return (this->m_v[0]);
			}

			if (t >= this->m_t[this->m_size - 1]) {
				return (this->m_v[this->m_size - 1]);
			}

			if (this->m_size == 2) {
				const T dt = this->m_t[1] - this->m_t[0];

				if (dt <= T(0)) {
					return (this->m_v[0]);
				}

				const T u1 = (t - this->m_t[0]) / dt;

				const V v0 = this->m_v[0];
				const V v1 = this->m_v[1];

				const V r = v0 + ((v1 - v0) * u1);

				return (r);
			}

			int i = this->GetKeyFrame(t);

			const T dt = this->m_t[i + 1] - this->m_t[i];

			if (dt <= T(0)) {
				return (this->m_v[i]);
			}

			const T u1 = (t - this->m_t[i]) / dt;
			const T u2 = u1 * u1;
			const T u3 = u2 * u1;

			if (i == 0) {
				//const T d1 = this->m_t[i + 2] - this->m_t[i];
				const T d1 = T(2);

				if (d1 <= T(0)) {
					return (this->m_v[i]);
				}

				const V v0 = this->m_v[i];
				const V v1 = this->m_v[i + 1];
				const V v20 = (this->m_v[i + 2] - v0) / d1;

				const V t0 = v0;
				const V t1 = (((v1 - v0) * T(3)) - v20) * T(0.5);
				const V t3 = (v0 - v1 + v20) * T(0.5) ;

				const V r = (t0) + (t1 * u1) + (t3 * u3);

				return (r);
			}
			else if (i == this->m_size - 2) {
				//const T d0 = this->m_t[i + 1] - this->m_t[i - 1];
				const T d0 = T(2);

				if (d0 <= T(0)) {
					return (this->m_v[i]);
				}

				const V v0 = this->m_v[i];
				const V v1 = this->m_v[i + 1];
				const V v11 = (v1 - this->m_v[i - 1]) / d0;

				const V t0 = v0;
				const V t1 = v11;
				const V t2 = (v1 - v0 - v11) * T(1.5);
				const V t3 = (v0 - v1 + v11) * T(0.5);

				const V r = (t0) + (t1 * u1) + (t2 * u2) + (t3 * u3);

				return (r);
			}
			else {
				//const T d0 = this->m_t[i + 1] - this->m_t[i - 1];
				const T d0 = T(2);

				if (d0 <= T(0)) {
					return (this->m_v[i]);
				}

				//const T d1 = this->m_t[i + 2] - this->m_t[i];
				const T d1 = T(2);

				if (d1 <= T(0)) {
					return (this->m_v[i]);
				}

				const V v0 = this->m_v[i];
				const V v1 = this->m_v[i + 1];
				const V v11 = (this->m_v[i + 1] - this->m_v[i - 1]) / d0;
				const V v20 = (this->m_v[i + 2] - this->m_v[i]) / d1;

				const V t0 = v0;
				const V t1 = v11;
				const V t2 = ((v1 - v0) * T(3)) - (v11 * T(2)) - (v20);
				const V t3 = ((v0 - v1) * T(2)) + (v11) + (v20);

				const V r = (t0) + (t1 * u1) + (t2 * u2) + (t3 * u3);

				return (r);
			}
		}

};



#endif /* __interpolator_h_ */
