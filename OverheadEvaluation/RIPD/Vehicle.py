import os
import glob
import json
import base64
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import ec
import random
import time
import socket
import threading

key_dir = "/Users/keyaohuang/Desktop/Implementation/RandomSeq/VehicleKeysSet/"
time_s = time.time()
def load_private_key(path):
    with open(path, "rb") as f:
        key_data = f.read()
    private_key = serialization.load_pem_private_key(key_data, password=None, backend=default_backend())
    return private_key

# Load Vehicle's long-term private key if needed
V_private_key_path = os.path.join(key_dir, "V_private_key.pem")
if os.path.exists(V_private_key_path):
    V_private_key = load_private_key(V_private_key_path)
else:
    V_private_key = None

# Scan and load pseudonym key pairs
pseudonym_priv_keys = []
pseudonym_pub_keys = []

pub_key_files = sorted(glob.glob(os.path.join(key_dir, "V_Pseudonym_public_key_*.pem")))

for pub_file in pub_key_files:
    index = pub_file.split("_")[-1].split(".")[0]
    priv_file = os.path.join(key_dir, f"V_Pseudonym_private_key_{index}.pem")
    if os.path.exists(priv_file):
        with open(pub_file, "rb") as f:
            pub_data = f.read()
        
        # Load corresponding keys
        pub_key = serialization.load_pem_public_key(pub_data, backend=default_backend())
        priv_key = load_private_key(priv_file)
        
        pseudonym_priv_keys.append(priv_key)
        # Store the public key in PEM string form
        pseudonym_pub_keys.append(pub_data.decode('utf-8'))

# Now split the keys and priv_keys into two halves
total_keys = len(pseudonym_pub_keys)
half = total_keys // 2

pca1_pub_keys = pseudonym_pub_keys[:half]
pca1_priv_keys = pseudonym_priv_keys[:half]

pca2_pub_keys = pseudonym_pub_keys[half:]
pca2_priv_keys = pseudonym_priv_keys[half:]

# Serialize and sign the first half
pca1_public_keys_json = json.dumps(pca1_pub_keys, ensure_ascii=False)
pca1_signatures = []
for priv_key in pca1_priv_keys:
    signature = priv_key.sign(
        pca1_public_keys_json.encode('utf-8'),
        ec.ECDSA(hashes.SHA256())
    )
    pca1_signatures.append(base64.b64encode(signature).decode('utf-8'))

# Serialize and sign the second half
pca2_public_keys_json = json.dumps(pca2_pub_keys, ensure_ascii=False)
pca2_signatures = []
for priv_key in pca2_priv_keys:
    signature = priv_key.sign(
        pca2_public_keys_json.encode('utf-8'),
        ec.ECDSA(hashes.SHA256())
    )
    pca2_signatures.append(base64.b64encode(signature).decode('utf-8'))

Id_req = 0
N = random.randint(0,10000)
t_now = time.time()
n_tkt = "xxxxxxxxx"
n_tkt_signature = "xxxxxxxx"

group1 = random.sample(range(1, 21), 10)
remaining_numbers = set(range(1, 21)) - set(group1)
group2 = random.sample(remaining_numbers, 10)
# Create request packs for each PCA
pca1_request_pack = {
    "Id_req": Id_req,
    "public_keys": pca1_pub_keys,
    "signatures": pca1_signatures,
    "Pseq":group1,
    "n_tkt": n_tkt,
    "n_tkt_signature": n_tkt_signature,
    "PCAs": [1, 2],
    "N": N,
    "t_now": t_now
}

pca2_request_pack = {
    "Id_req": Id_req,
    "public_keys": pca2_pub_keys,
    "signatures": pca2_signatures,
    "Pseq":group2,
    "n_tkt": n_tkt,
    "n_tkt_signature": n_tkt_signature,
    "PCAs": [1, 2],
    "N": N,
    "t_now": t_now
}

pca1_message = json.dumps(pca1_request_pack, ensure_ascii=False)
pca2_message = json.dumps(pca2_request_pack, ensure_ascii=False)

def communicate_with_pca(host, port, message, pseudonym_pub_keys_subset):
    """Thread function to communicate with a given PCA and verify its response."""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.sendall(message.encode('utf-8'))
        print(f"[Thread-{port}] Message sent to PCA at {host}:{port}, waiting for response...")

        data = s.recv(40960)
        if data:
            response = json.loads(data.decode('utf-8'))
            print(f"[Thread-{port}] Response received from PCA:", response)

            if response.get("status") == "OK":
                pca_public_key_pem = response.get("pca_public_key")
                pca_signatures = response.get("pca_signatures", [])

                pca_public_key = serialization.load_pem_public_key(
                    pca_public_key_pem.encode('utf-8'), 
                    backend=default_backend()
                )

                all_pca_signatures_valid = True
                for i, pca_sig_b64 in enumerate(pca_signatures):
                    pca_sig = base64.b64decode(pca_sig_b64)
                    pub_key_str = pseudonym_pub_keys_subset[i].encode('utf-8')
                    try:
                        pca_public_key.verify(
                            pca_sig,
                            pub_key_str,
                            ec.ECDSA(hashes.SHA256())
                        )
                        print(f"[Thread-{port}] PCA signature for public key {i+1} verified successfully.")
                    except Exception:
                        print(f"[Thread-{port}] PCA signature verification failed for public key {i+1}.")
                        all_pca_signatures_valid = False
                
                if all_pca_signatures_valid:
                    print(f"[Thread-{port}] All PCA signatures verified. Vehicle can trust this PCA.")
                else:
                    print(f"[Thread-{port}] Some PCA signatures failed verification.")

            else:
                print(f"[Thread-{port}] PCA returned FAIL status, reason:", response.get("reason", "Unknown"))
        else:
            print(f"[Thread-{port}] No response received from PCA.")

# Assume PCA1 at 127.0.0.1:5000 and PCA2 at 127.0.0.1:5001
PCA1_HOST, PCA1_PORT = '192.168.209.157', 5000
PCA2_HOST, PCA2_PORT = '192.168.209.155', 5000

# Create threads, sending different halves to different PCAs
thread1 = threading.Thread(target=communicate_with_pca, args=(PCA1_HOST, PCA1_PORT, pca1_message, pca1_pub_keys))
thread2 = threading.Thread(target=communicate_with_pca, args=(PCA2_HOST, PCA2_PORT, pca2_message, pca2_pub_keys))

# Start threads
thread1.start()
thread2.start()

# Wait for both threads to complete
thread1.join()
thread2.join()

print("Both communications with PCA1 and PCA2 are done.")
time_e = time.time()
print(time_e - time_s)