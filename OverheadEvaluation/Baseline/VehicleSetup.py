from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import ec

# Number of pseudonym key pairs to generate
n = 40  # Change this to your desired number of pseudonym key pairs
# V long-term public-private key pair
V_Lk = ec.generate_private_key(ec.SECP256R1(), default_backend())
V_LK = V_Lk.public_key()

V_Lk_pem = V_Lk.private_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PrivateFormat.PKCS8,
    encryption_algorithm=serialization.NoEncryption()
)

with open('/Users/keyaohuang/Desktop/Implementation/Baseline/VehicleKeysSet/V_private_key.pem', 'wb') as f:
    f.write(V_Lk_pem)

V_LK_pem = V_LK.public_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PublicFormat.SubjectPublicKeyInfo
)

with open('/Users/keyaohuang/Desktop/Implementation/Baseline/VehicleKeysSet/V_public_key.pem', 'wb') as f:
    f.write(V_LK_pem)

# Generate n pseudonym key pairs
for i in range(1, n + 1):
    P_sk = ec.generate_private_key(ec.SECP256R1(), default_backend())
    P_pK = P_sk.public_key()

    P_sk_pem = P_sk.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption()
    )

    # Save pseudonym private key
    with open(f'/Users/keyaohuang/Desktop/Implementation/Baseline/VehicleKeysSet/V_Pseudonym_private_key_{i}.pem', 'wb') as f:
        f.write(P_sk_pem)

    P_pK_pem = P_pK.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )

    # Save pseudonym public key
    with open(f'/Users/keyaohuang/Desktop/Implementation/Baseline/VehicleKeysSet/V_Pseudonym_public_key_{i}.pem', 'wb') as f:
        f.write(P_pK_pem)
