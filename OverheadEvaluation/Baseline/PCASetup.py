from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import ec


V_Lk = ec.generate_private_key(ec.SECP256R1(), default_backend())
V_LK = V_Lk.public_key()

V_Lk_pem = V_Lk.private_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PrivateFormat.PKCS8,
    encryption_algorithm=serialization.NoEncryption()
)

with open('/PCA1/PCAKeysSet/PCA_private_key.pem', 'wb') as f:
    f.write(V_Lk_pem)

V_LK_pem = V_LK.public_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PublicFormat.SubjectPublicKeyInfo
)

with open('/PCA1/PCAKeysSet/PCA_public_key.pem', 'wb') as f:
    f.write(V_LK_pem)