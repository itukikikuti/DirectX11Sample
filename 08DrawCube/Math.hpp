struct Float2 : public DirectX::XMFLOAT2
{
	PUBLIC Float2() : DirectX::XMFLOAT2()
	{
	}
	PUBLIC Float2(float x, float y) : DirectX::XMFLOAT2(x, y)
	{
	}
	PUBLIC Float2(float value) : DirectX::XMFLOAT2(value, value)
	{
	}
	PUBLIC Float2(const DirectX::XMVECTOR& vector) : DirectX::XMFLOAT2()
	{
		DirectX::XMStoreFloat2(this, vector);
	}
	PUBLIC Float2& operator=(const DirectX::XMVECTOR& vector)
	{
		DirectX::XMStoreFloat2(this, vector);
		return *this;
	}
	PUBLIC operator DirectX::XMVECTOR() const noexcept
	{
		return DirectX::XMLoadFloat2(this);
	}
	PUBLIC Float2 operator+() const
	{
		return Float2(this->x, this->y);
	}
	PUBLIC Float2 operator-() const
	{
		return Float2(-this->x, -this->y);
	}
	PUBLIC Float2& operator=(const Float2& value)
	{
		x = value.x;
		y = value.y;
		return *this;
	}
	PUBLIC Float2& operator=(const float& value)
	{
		x = value;
		y = value;
		return *this;
	}
	PUBLIC Float2& operator+=(const Float2& value)
	{
		x += value.x;
		y += value.y;
		return *this;
	}
	PUBLIC Float2& operator+=(const float& value)
	{
		x += value;
		y += value;
		return *this;
	}
	PUBLIC Float2& operator-=(const Float2& value)
	{
		x -= value.x;
		y -= value.y;
		return *this;
	}
	PUBLIC Float2& operator-=(const float& value)
	{
		x -= value;
		y -= value;
		return *this;
	}
	PUBLIC Float2& operator*=(const Float2& value)
	{
		x *= value.x;
		y *= value.y;
		return *this;
	}
	PUBLIC Float2& operator*=(const float& value)
	{
		x *= value;
		y *= value;
		return *this;
	}
	PUBLIC Float2& operator/=(const Float2& value)
	{
		x /= value.x;
		y /= value.y;
		return *this;
	}
	PUBLIC Float2& operator/=(const float& value)
	{
		x /= value;
		y /= value;
		return *this;
	}
};

Float2 operator+(const Float2& t1, const Float2& t2)
{
	return Float2(t1) += t2;
}
Float2 operator+(const Float2& t1, const float& t2)
{
	return Float2(t1) += t2;
}
Float2 operator-(const Float2& t1, const Float2& t2)
{
	return Float2(t1) -= t2;
}
Float2 operator-(const Float2& t1, const float& t2)
{
	return Float2(t1) -= t2;
}
Float2 operator*(const Float2& t1, const Float2& t2)
{
	return Float2(t1) *= t2;
}
Float2 operator*(const Float2& t1, const float& t2)
{
	return Float2(t1) *= t2;
}
Float2 operator/(const Float2& t1, const Float2& t2)
{
	return Float2(t1) /= t2;
}
Float2 operator/(const Float2& t1, const float& t2)
{
	return Float2(t1) /= t2;
}

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

struct Float4 : public DirectX::XMFLOAT4
{
	PUBLIC Float4() : DirectX::XMFLOAT4()
	{
	}
	PUBLIC Float4(float x, float y, float z, float w) : DirectX::XMFLOAT4(x, y, z, w)
	{
	}
	PUBLIC Float4(float value) : DirectX::XMFLOAT4(value, value, value, value)
	{
	}
	PUBLIC Float4(const DirectX::XMVECTOR& vector) : DirectX::XMFLOAT4()
	{
		DirectX::XMStoreFloat4(this, vector);
	}
	PUBLIC Float4& operator=(const DirectX::XMVECTOR& vector)
	{
		DirectX::XMStoreFloat4(this, vector);
		return *this;
	}
	PUBLIC operator DirectX::XMVECTOR() const noexcept
	{
		return DirectX::XMLoadFloat4(this);
	}
	PUBLIC Float4 operator+() const
	{
		return Float4(this->x, this->y, this->z, this->w);
	}
	PUBLIC Float4 operator-() const
	{
		return Float4(-this->x, -this->y, -this->z, -this->w);
	}
	PUBLIC Float4& operator=(const Float4& value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
		w = value.w;
		return *this;
	}
	PUBLIC Float4& operator=(const float& value)
	{
		x = value;
		y = value;
		z = value;
		w = value;
		return *this;
	}
	PUBLIC Float4& operator+=(const Float4& value)
	{
		x += value.x;
		y += value.y;
		z += value.z;
		w += value.w;
		return *this;
	}
	PUBLIC Float4& operator+=(const float& value)
	{
		x += value;
		y += value;
		z += value;
		w += value;
		return *this;
	}
	PUBLIC Float4& operator-=(const Float4& value)
	{
		x -= value.x;
		y -= value.y;
		z -= value.z;
		w -= value.w;
		return *this;
	}
	PUBLIC Float4& operator-=(const float& value)
	{
		x -= value;
		y -= value;
		z -= value;
		w -= value;
		return *this;
	}
	PUBLIC Float4& operator*=(const Float4& value)
	{
		x *= value.x;
		y *= value.y;
		z *= value.z;
		w *= value.w;
		return *this;
	}
	PUBLIC Float4& operator*=(const float& value)
	{
		x *= value;
		y *= value;
		z *= value;
		w *= value;
		return *this;
	}
	PUBLIC Float4& operator/=(const Float4& value)
	{
		x /= value.x;
		y /= value.y;
		z /= value.z;
		w /= value.w;
		return *this;
	}
	PUBLIC Float4& operator/=(const float& value)
	{
		x /= value;
		y /= value;
		z /= value;
		w /= value;
		return *this;
	}
};

Float4 operator+(const Float4& t1, const Float4& t2)
{
	return Float4(t1) += t2;
}
Float4 operator+(const Float4& t1, const float& t2)
{
	return Float4(t1) += t2;
}
Float4 operator-(const Float4& t1, const Float4& t2)
{
	return Float4(t1) -= t2;
}
Float4 operator-(const Float4& t1, const float& t2)
{
	return Float4(t1) -= t2;
}
Float4 operator*(const Float4& t1, const Float4& t2)
{
	return Float4(t1) *= t2;
}
Float4 operator*(const Float4& t1, const float& t2)
{
	return Float4(t1) *= t2;
}
Float4 operator/(const Float4& t1, const Float4& t2)
{
	return Float4(t1) /= t2;
}
Float4 operator/(const Float4& t1, const float& t2)
{
	return Float4(t1) /= t2;
}

struct Vertex
{
	PUBLIC Float3 position;
	PUBLIC Float3 normal;
};
