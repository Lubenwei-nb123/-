#coding:utf-8

#y^2=x^3+7
A = 0
B = 7

# 有限域的阶
P = 115792089237316195423570985008687907853269984665640564039457584007908834671663
# 椭圆曲线的阶
N = 115792089237316195423570985008687907852837564279074904382605163141518161494337

def Exgcd(j, k):
    if j == k:
        return (j, 1, 0)
    else:
        i = 0
        j_seq = [j]
        k_seq = [k]
        q_seq = []
        r_seq = []

        flag = False

        while not (flag):
            q_seq.append(k_seq[i]//j_seq[i])
            r_seq.append(k_seq[i]%j_seq[i])
            k_seq.append(j_seq[i])
            j_seq.append(r_seq[i])
            i += 1
            if r_seq[i-1] == 0:
                flag = True
        i -= 1
        gcd = j_seq[i]
        x_array = [1]
        y_array = [0]

        i -= 1
        total_steps = i

        while i >= 0:
            y_array.append(x_array[total_steps-i])
            x_array.append(y_array[total_steps-i] - q_seq[i]*x_array[total_steps-i])
            i -= 1

        return (gcd, x_array[-1], y_array[-1])
#求模逆
def mod_inverse(j, n):
    (gcd, x, y) = Exgcd(j, n)

    if gcd == 1:
        return x % n
    else:
        return -1


'''椭圆曲线加、乘'''
def elliptic_add(p, q):
    if p == 0 and q == 0: return 0
    elif p == 0: return q
    elif q == 0: return p
    else:
        # Swap p and q if px > qx.
        if p[0] > q[0]:
            temp = p
            p = q
            q = temp
        r = []
        slope = (q[1] - p[1])*mod_inverse(q[0] - p[0], P) % P

        r.append((slope**2 - p[0] - q[0]) % P)
        r.append((slope*(p[0] - r[0]) - p[1]) % P)

        return (r[0], r[1])


def elliptic_double(p):
    r = []

    slope = (3*p[0]**2 + A) * mod_inverse(2*p[1], P) % P

    r.append((slope**2 - 2*p[0])%P)
    r.append((slope*(p[0] - r[0]) - p[1])%P)

    return (r[0], r[1])

def elliptic_multiply(s, p):
    n = p
    r = 0 # 0 representing a point at infinity

    s_binary = bin(s)[2:] # convert s to binary and remove the "0b" in the beginning
    s_length = len(s_binary)

    for i in reversed(range(s_length)):
        if s_binary[i] == '1':
            r = elliptic_add(r, n)
        n = elliptic_double(n)

    return r




