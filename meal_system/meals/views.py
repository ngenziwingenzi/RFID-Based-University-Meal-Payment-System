from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import json
from .models import Student  # Assuming Student has a 'uid' and 'balance'
from meals.models import Log  # ✅ Use Log model for both actions

@csrf_exempt
def admin_refill(request):
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            uid = data.get('uid', '').lower()
            amount = data.get('amount')

            if not uid:
                return JsonResponse({'error': 'No UID provided'}, status=400)
            if amount is None:
                return JsonResponse({'error': 'No amount provided'}, status=400)

            try:
                student = Student.objects.get(uid=uid)
                student.balance += int(amount)
                student.save()

                # ✅ Log the refill
                Log.objects.create(
                    student=student,
                    amount=int(amount),
                    action='refill'
                )

                return JsonResponse({
                    'message': f'Balance refilled by {amount} successfully',
                    'new_balance': student.balance
                })
            except Student.DoesNotExist:
                return JsonResponse({'error': 'Card not found'}, status=404)

        except Exception as e:
            return JsonResponse({'error': str(e)}, status=500)

    return JsonResponse({'error': 'Invalid request method'}, status=405)


@csrf_exempt
def student_scan(request):
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            uid = data.get('uid', '').lower()

            if not uid:
                return JsonResponse({'error': 'No UID provided'}, status=400)

            try:
                student = Student.objects.get(uid=uid)

                if student.balance >= 1000:  # Assuming meal cost is 1000
                    student.balance -= 1000
                    student.save()

                    # ✅ Log the meal transaction
                    Log.objects.create(
                        student=student,
                        amount=-1000,
                        action='meal'
                    )

                    return JsonResponse({
                        'message': 'Meal accepted',
                        'new_balance': student.balance
                    })
                else:
                    return JsonResponse({'message': 'Insufficient balance'}, status=200)

            except Student.DoesNotExist:
                return JsonResponse({'error': 'Card not found'}, status=404)

        except Exception as e:
            return JsonResponse({'error': str(e)}, status=500)

    return JsonResponse({'error': 'Invalid request method'}, status=405)
