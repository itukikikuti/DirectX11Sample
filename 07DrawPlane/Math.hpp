struct Float3 : public DirectX::XMFLOAT3
{
	PUBLIC Float3() : DirectX::XMFLOAT3()
	{
	}
	PUBLIC Float3(float x, float y, float z) : DirectX::XMFLOAT3(x, y, z)
	{
	}
	PUBLIC Float3(float value) : DirectX::XMFLOAT3(value, value, value)
	{
	}
	PUBLIC Float3(const DirectX::XMVECTOR& vector) : DirectX::XMFLOAT3()
	{
		DirectX::XMStoreFloat3(this, vector);
	}
	PUBLIC Float3& operator=(const DirectX::XMVECTOR& vector)
	{
		DirectX::XMStoreFloat3(this, vector);
		return *this;
	}
	PUBLIC operator DirectX::XMVECTOR() const noexcept
	{
		return DirectX::XMLoadFloat3(this);
	}
	PUBLIC Float3 operator+() const
	{
		return Float3(this->x, this->y, this->z);
	}
	PUBLIC Float3 operator-() const
	{
		return Float3(-this->x, -this->y, -this->z);
	}
	PUBLIC Float3& operator=(const Float3& value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
		return *this;
	}
	PUBLIC Float3& operator=(const float& value)
	{
		x = value;
		y = value;
		z = value;
		return *this;
	}
	PUBLIC Float3& operator+=(const Float3& value)
	{
		x += value.x;
		y += value.y;
		z += value.z;
		return *this;
	}
	PUBLIC Float3& operator+=(const float& value)
	{
		x += value;
		y += value;
		z += value;
		return *this;
	}
	PUBLIC Float3& operator-=(const Float3& value)
	{
		x -= value.x;
		y -= value.y;
		z -= value.z;
		return *this;
	}
	PUBLIC Float3& operator-=(const float& value)
	{
		x -= value;
		y -= value;
		z -= value;
		return *this;
	}
	PUBLIC Float3& operator*=(const Float3& value)
	{
		x *= value.x;
		y *= value.y;
		z *= value.z;
		return *this;
	}
	PUBLIC Float3& operator*=(const float& value)
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}
	PUBLIC Float3& operator/=(const Float3& value)
	{
		x /= value.x;
		y /= value.y;
		z /= value.z;
		return *this;
	}
	PUBLIC Float3& operator/=(const float& value)
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}
};

Float3 operator+(const Float3& t1, const Float3& t2)
{
	return Float3(t1) += t2;
}
Float3 operator+(const Float3& t1, const float& t2)
{
	return Float3(t1) += t2;
}
Float3 operator-(const Float3& t1, const Float3& t2)
{
	return Float3(t1) -= t2;
}
Float3 operator-(const Float3& t1, const float& t2)
{
	return Float3(t1) -= t2;
}
Float3 operator*(const Float3& t1, const Float3& t2)
{
	return Float3(t1) *= t2;
}
Float3 operator*(const Float3& t1, const float& t2)
{
	return Float3(t1) *= t2;
}
Float3 operator/(const Float3& t1, const Float3& t2)
{
	return Float3(t1) /= t2;
}
Float3 operator/(const Float3& t1, const float& t2)
{
	return Float3(t1) /= t2;
}

struct Vertex
{
	PUBLIC Float3 position;
	PUBLIC Float3 color;
};
